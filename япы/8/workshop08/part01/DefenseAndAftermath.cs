using System;
using System.Reflection;
using System.Reflection.Emit;

namespace part01
{
    [GameAttribute]
    public class DefenseAndAftermath
    {
        // IL delegates
        private static readonly Func<int, int> _halfDamage = BuildHalfDamage();
        private static readonly Action<BattleContext> _postBattle = BuildPostBattleDrain();

        [CombatSkill("ShieldWall", TriggerType.OnDefense, 100)]
        public void OnDefense(BattleContext ctx)
        {
            ctx.DamageDealt = _halfDamage(ctx.DamageDealt);
            Console.WriteLine("[System] ShieldWall: damage halved.");
        }

        [CombatSkill("Aftershock", TriggerType.PostBattle, 1)]
        public void PostBattle(BattleContext ctx)
        {
            _postBattle(ctx);
            Console.WriteLine("[System] Aftershock: both units lose 20 HP.");
        }

        private static Func<int, int> BuildHalfDamage()
        {
            var dm = new DynamicMethod(
                name: "HalfDamage",
                returnType: typeof(int),
                parameterTypes: new[] { typeof(int) });

            ILGenerator il = dm.GetILGenerator();

            // damage => damage / 2
            il.Emit(OpCodes.Ldarg_0);
            il.Emit(OpCodes.Ldc_I4_2);
            il.Emit(OpCodes.Div);
            il.Emit(OpCodes.Ret);

            return (Func<int, int>)dm.CreateDelegate(typeof(Func<int, int>));
        }

        private static Action<BattleContext> BuildPostBattleDrain()
        {
            var dm = new DynamicMethod(
                name: "PostBattleDrain",
                returnType: typeof(void),
                parameterTypes: new[] { typeof(BattleContext) },
                m: typeof(DefenseAndAftermath).Module,
                skipVisibility: true);

            ILGenerator il = dm.GetILGenerator();

            MethodInfo getAttacker = typeof(BattleContext)
                .GetProperty(nameof(BattleContext.Attacker))!
                .GetGetMethod()!;

            MethodInfo getDefender = typeof(BattleContext)
                .GetProperty(nameof(BattleContext.Defender))!
                .GetGetMethod()!;

            MethodInfo getHp = typeof(UnitStats)
                .GetProperty(nameof(UnitStats.Hp))!
                .GetGetMethod()!;

            MethodInfo setHp = typeof(UnitStats)
                .GetProperty(nameof(UnitStats.Hp))!
                .GetSetMethod()!;

            // attacker.Hp = attacker.Hp - 20
            il.Emit(OpCodes.Ldarg_0);
            il.Emit(OpCodes.Call, getAttacker); // attacker
            il.Emit(OpCodes.Dup);               // attacker, attacker
            il.Emit(OpCodes.Call, getHp);       // attacker, hp
            il.Emit(OpCodes.Ldc_I4_S, 20);      // attacker, hp, 20
            il.Emit(OpCodes.Sub);               // attacker, hp-20
            il.Emit(OpCodes.Call, setHp);       // setHp(attacker, hp-20)

            // defender.Hp = defender.Hp - 20
            il.Emit(OpCodes.Ldarg_0);
            il.Emit(OpCodes.Call, getDefender); // defender
            il.Emit(OpCodes.Dup);               // defender, defender
            il.Emit(OpCodes.Call, getHp);       // defender, hp
            il.Emit(OpCodes.Ldc_I4_S, 20);      // defender, hp, 20
            il.Emit(OpCodes.Sub);               // defender, hp-20
            il.Emit(OpCodes.Call, setHp);

            il.Emit(OpCodes.Ret);

            return (Action<BattleContext>)dm.CreateDelegate(typeof(Action<BattleContext>));
        }
    }
}
