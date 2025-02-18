/**
 * @file
 * @brief Brand/ench/etc effects that might alter something in an
 *             unexpected way.
**/

#pragma once

#include "actor.h"
#include "beh-type.h"
#include "mgen-data.h"
#include "mon-util.h"
#include "monster.h"

struct bolt;

class final_effect
{
public:
    virtual ~final_effect() {}

    virtual bool mergeable(const final_effect &a) const = 0;
    virtual void merge(const final_effect &)
    {
    }

    virtual void fire() = 0;

protected:
    static void schedule(final_effect *eff);

    mid_t att, def;
    coord_def posn;
    actor *attacker() const { return actor_by_mid(att); }
    actor *defender() const { return actor_by_mid(def); }

    final_effect(const actor *attack, const actor *defend, const coord_def &pos)
        : att(attack ? attack->mid : 0),
          def(defend ? defend->mid : 0),
          posn(pos)
    {
    }
};

class mirror_damage_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &a) const override;
    void merge(const final_effect &a) override;
    void fire() override;

    static void schedule(const actor *attack, const actor *defend, int dam)
    {
        final_effect::schedule(new mirror_damage_fineff(attack, defend, dam));
    }
protected:
    mirror_damage_fineff(const actor *attack, const actor *defend, int dam)
        : final_effect(attack, defend, coord_def()), damage(dam)
    {
    }
    int damage;
};

class anguish_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &a) const override;
    void merge(const final_effect &a) override;
    void fire() override;

    static void schedule(const actor *attack, int dam)
    {
        final_effect::schedule(new anguish_fineff(attack, dam));
    }
protected:
    anguish_fineff(const actor *attack, int dam)
        : final_effect(attack, nullptr, coord_def()), damage(dam*2)
    {
    }
    int damage;
};

class ru_retribution_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &a) const override;
    void merge(const final_effect &a) override;
    void fire() override;

    static void schedule(const actor *attack, const actor *defend, int dam)
    {
        final_effect::schedule(new ru_retribution_fineff(attack, defend, dam));
    }
protected:
    ru_retribution_fineff(const actor *attack, const actor *defend, int dam)
        : final_effect(attack, defend, coord_def()), damage(dam)
    {
    }
    int damage;
};

class trample_follow_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &a) const override;
    void fire() override;

    static void schedule(const actor *attack, const coord_def &pos)
    {
        final_effect::schedule(new trample_follow_fineff(attack, pos));
    }
protected:
    trample_follow_fineff(const actor *attack, const coord_def &pos)
        : final_effect(attack, 0, pos)
    {
    }
};

class blink_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &a) const override;
    void fire() override;

    static void schedule(const actor *blinker, const actor *other = nullptr)
    {
        final_effect::schedule(new blink_fineff(blinker, other));
    }
protected:
    blink_fineff(const actor *blinker, const actor *o)
        : final_effect(o, blinker, coord_def())
    {
    }
};

class teleport_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &a) const override;
    void fire() override;

    static void schedule(const actor *defend)
    {
        final_effect::schedule(new teleport_fineff(defend));
    }
protected:
    teleport_fineff(const actor *defend)
        : final_effect(0, defend, coord_def())
    {
    }
};

class trj_spawn_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &a) const override;
    void merge(const final_effect &a) override;
    void fire() override;

    static void schedule(const actor *attack, const actor *defend,
                         const coord_def &pos, int dam)
    {
        final_effect::schedule(new trj_spawn_fineff(attack, defend, pos, dam));
    }
protected:
    trj_spawn_fineff(const actor *attack, const actor *defend,
                     const coord_def &pos, int dam)
        : final_effect(attack, defend, pos), damage(dam)
    {
    }
    int damage;
};

class blood_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &a) const override;
    void fire() override;
    void merge(const final_effect &a) override;

    static void schedule(const actor *defend, const coord_def &pos,
                         int blood_amount)
    {
        final_effect::schedule(new blood_fineff(defend, pos, blood_amount));
    }
protected:
    blood_fineff(const actor *defend, const coord_def &pos, int blood_amount)
        : final_effect(0, 0, pos), mtype(defend->type), blood(blood_amount)
    {
    }
    monster_type mtype;
    int blood;
};

class deferred_damage_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &a) const override;
    void merge(const final_effect &a) override;
    void fire() override;

    static void schedule(const actor *attack, const actor *defend,
                         int dam, bool attacker_effects, bool fatal = true)
    {
        final_effect::schedule(
            new deferred_damage_fineff(attack, defend, dam, attacker_effects,
                                       fatal));
    }
protected:
    deferred_damage_fineff(const actor *attack, const actor *defend,
                           int dam, bool _attacker_effects, bool _fatal)
        : final_effect(attack, defend, coord_def()),
          damage(dam), attacker_effects(_attacker_effects), fatal(_fatal)
    {
    }
    int damage;
    bool attacker_effects;
    bool fatal;
};

class starcursed_merge_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &a) const override;
    void fire() override;

    static void schedule(const actor *merger)
    {
        final_effect::schedule(new starcursed_merge_fineff(merger));
    }
protected:
    starcursed_merge_fineff(const actor *merger)
        : final_effect(0, merger, coord_def())
    {
    }
};

class shock_serpent_discharge_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &a) const override;
    void merge(const final_effect &a) override;
    void fire() override;

    static void schedule(const actor *serpent, actor &oppressor,
                         coord_def pos, int pow)
    {
        final_effect::schedule(new shock_serpent_discharge_fineff(serpent,
                                                                  oppressor,
                                                                  pos, pow));
    }
protected:
    shock_serpent_discharge_fineff(const actor *serpent, actor &rudedude,
                                   coord_def pos, int pow)
        : final_effect(0, serpent, coord_def()), oppressor(rudedude),
          position(pos), power(pow),
        attitude(mons_attitude(*serpent->as_monster()))
    {
    }
    actor &oppressor;
    coord_def position;
    int power;
    mon_attitude_type attitude;
};

class explosion_fineff : public final_effect
{
public:
    // One explosion at a time, please.
    bool mergeable(const final_effect &) const override { return false; }
    void fire() override;

    static void schedule(bolt &beam, string boom, string sanct,
                         bool inner_flame, const actor* flame_agent)
    {
        final_effect::schedule(new explosion_fineff(beam, boom, sanct,
                                                    inner_flame, flame_agent));
    }
protected:
    explosion_fineff(const bolt &beem, string boom, string sanct,
                     bool flame, const actor* agent)
        : final_effect(0, 0, coord_def()), beam(beem),
          boom_message(boom), sanctuary_message(sanct),
          inner_flame(flame), flame_agent(agent)
    {
    }
    bolt beam;
    string boom_message;
    string sanctuary_message;
    bool inner_flame;
    const actor* flame_agent;
};

// A fineff that triggers a daction; otherwise the daction
// occurs immediately (and then later) -- this might actually
// be too soon in some cases.
class delayed_action_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &a) const override;
    virtual void fire() override;

    static void schedule(daction_type action, const string &final_msg)
    {
        final_effect::schedule(new delayed_action_fineff(action, final_msg));
    }
protected:
    delayed_action_fineff(daction_type _action, const string &_final_msg)
        : final_effect(0, 0, coord_def()),
          action(_action), final_msg(_final_msg)
    {
    }
    daction_type action;
    string final_msg;
};

class kirke_death_fineff : public delayed_action_fineff
{
public:
    void fire() override;

    static void schedule(const string &final_msg)
    {
        final_effect::schedule(new kirke_death_fineff(final_msg));
    }
protected:
    kirke_death_fineff(const string & _final_msg)
        : delayed_action_fineff(DACT_KIRKE_HOGS, _final_msg)
    {
    }
};

class rakshasa_clone_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &a) const override;
    void fire() override;

    static void schedule(const actor *defend, const coord_def &pos)
    {
        final_effect::schedule(new rakshasa_clone_fineff(defend, pos));
    }
protected:
    rakshasa_clone_fineff(const actor *defend, const coord_def &pos)
        : final_effect(0, defend, pos)
    {
    }
    int damage;
};

class bennu_revive_fineff : public final_effect
{
public:
    // Each trigger is from the death of a different bennu---no merging.
    bool mergeable(const final_effect &) const override { return false; }
    void fire() override;

    static void schedule(coord_def pos, int revives, beh_type attitude,
                         unsigned short foe, bool duel)
    {
        final_effect::schedule(new bennu_revive_fineff(pos, revives, attitude, foe, duel));
    }
protected:
    bennu_revive_fineff(coord_def pos, int _revives, beh_type _att,
                        unsigned short _foe, bool _duel)
        : final_effect(0, 0, pos), revives(_revives), attitude(_att), foe(_foe),
          duel(_duel)
    {
    }
    int revives;
    beh_type attitude;
    unsigned short foe;
    bool duel;
};

class avoided_death_fineff : public final_effect
{
public:
    // Each trigger is from the death of a different monster---no merging.
    bool mergeable(const final_effect &) const override { return false; }
    void fire() override;

    static void schedule(monster * mons)
    {
        // pretend to be dead until our revival, to prevent
        // sequencing errors from inadvertently making us change alignment
        const int realhp = mons->hit_points;
        mons->hit_points = -realhp;
        final_effect::schedule(new avoided_death_fineff(mons, realhp));
    }
protected:
    avoided_death_fineff(const actor * _def, int _hp)
        : final_effect(0, _def, coord_def()), hp(_hp)
    {
    }
    int hp;
};

class infestation_death_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &) const override { return false; }
    void fire() override;

    static void schedule(coord_def pos, const string &name)
    {
        final_effect::schedule(new infestation_death_fineff(pos, name));
    }
protected:
    infestation_death_fineff(coord_def pos, const string &_name)
        : final_effect(0, 0, pos), name(_name)
    {
    }
    string name;
};

class make_derived_undead_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &) const override { return false; }
    void fire() override;

    static void schedule(coord_def pos, mgen_data mg, int xl,
                         const string &agent, const string &msg)
    {
        final_effect::schedule(new make_derived_undead_fineff(pos, mg, xl, agent, msg));
    }
protected:
    make_derived_undead_fineff(coord_def pos, mgen_data _mg, int _xl,
                               const string &_agent, const string &_msg)
        : final_effect(0, 0, pos), mg(_mg), experience_level(_xl),
          agent(_agent), message(_msg)
    {
    }
    mgen_data mg;
    int experience_level;
    string agent;
    string message;
};

class mummy_death_curse_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &) const override { return false; }
    void fire() override;

    static void schedule(const actor * attack, string name, killer_type killer, int pow)
    {
        final_effect::schedule(new mummy_death_curse_fineff(attack, name, killer, pow));
    }
protected:
    mummy_death_curse_fineff(const actor * attack, string _name, killer_type _killer, int _pow)
        : final_effect(fixup_attacker(attack), 0, coord_def()), name(_name),
          killer(_killer), pow(_pow)
    {
    }
    const actor *fixup_attacker(const actor *a);

    string name;
    killer_type killer;
    int pow;
};

class summon_dismissal_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &fe) const override;
    void merge(const final_effect &) override;
    void fire() override;

    static void schedule(const actor * _defender)
    {
        final_effect::schedule(new summon_dismissal_fineff(_defender));
    }
protected:
    summon_dismissal_fineff(const actor * _defender)
        : final_effect(0, _defender, coord_def())
    {
    }
};

class spectral_weapon_fineff : public final_effect
{
public:
    bool mergeable(const final_effect &) const override { return false; };
    void fire() override;

    static void schedule(const actor &attack, const actor &defend)
    {
        final_effect::schedule(new spectral_weapon_fineff(attack, defend));
    }
protected:
    spectral_weapon_fineff(const actor &attack, const actor &defend)
        : final_effect(&attack, &defend, coord_def())
    {
    }
};

void fire_final_effects();
