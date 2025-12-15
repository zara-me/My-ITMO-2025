using System;
using System.Reflection;

namespace part01
{
    internal class Program
    {
        static void Main()
        {
            var engine = new SkillEngine();
            engine.RegisterAssembly(Assembly.GetExecutingAssembly());

            var context = new BattleContext
            {
                DamageDealt = 100,
                Attacker = new UnitStats { Hp = 50 },
                Defender = new UnitStats { Hp = 100 }
            };

            Console.WriteLine("--- Starting Defense Phase ---");
            engine.ExecutePipeline(TriggerType.OnDefense, context);

            Console.WriteLine("--- Starting Attack Phase ---");
            engine.ExecutePipeline(TriggerType.OnAttack, context);

            Console.WriteLine("--- Starting PostBattle Phase ---");
            engine.ExecutePipeline(TriggerType.PostBattle, context);

            Console.WriteLine($"Final DamageDealt: {context.DamageDealt}");
            Console.WriteLine($"Attacker Final HP: {context.Attacker.Hp}");
            Console.WriteLine($"Defender Final HP: {context.Defender.Hp}");
        }
    }
}
