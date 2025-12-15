using System;
using System.Threading;

namespace AutoBattler
{
    public class AutoBattler
    {
        private Hero a;
        private Hero b;
        private Random rand = new Random();

        public AutoBattler(Hero one, Hero two)
        {
            a = one;
            b = two;
        }

        public void StartBattle()
        {
            int turn = 0;
            ConsoleOutput.Log("=== BATTLE START ===", ConsoleColor.Yellow);

            while (a.HP > 0 && b.HP > 0)
            {
                turn++;
                Hero attacker = rand.Next(0, 2) == 0 ? a : b;
                Hero defender = attacker == a ? b : a;
                bool useSpecial = (turn % 3 == 0);

                if (useSpecial)
                {
                    ConsoleOutput.Log($"Turn {turn}: {attacker.Name} uses SPECIAL on {defender.Name}!", ConsoleColor.Cyan);
                    attacker.SpecialAbility(defender);
#pragma warning disable CA1416 // Validate platform compatibility
                    try { Console.Beep(1000, 180); } catch { }
#pragma warning restore CA1416 // Validate platform compatibility
                }
                else
                {
                    ConsoleOutput.Log($"Turn {turn}: {attacker.Name} attacks {defender.Name} for {attacker.Damage} dmg.", ConsoleColor.Green);
                    defender.TakeDamage(attacker.Damage);
#pragma warning disable CA1416 // Validate platform compatibility
                    try { Console.Beep(700, 120); } catch { }
#pragma warning restore CA1416 // Validate platform compatibility
                }

                ConsoleOutput.Log($"Status: {a}", ConsoleColor.DarkRed);
                ConsoleOutput.Log($"Status: {b}", ConsoleColor.DarkMagenta);

                Thread.Sleep(200);
            }

            if (a.HP == 0 && b.HP == 0)
                ConsoleOutput.Log("DRAW! Both heroes fell.", ConsoleColor.Yellow);
            else if (a.HP == 0)
                ConsoleOutput.Log($"{b.Name} wins!", ConsoleColor.Yellow);
            else
                ConsoleOutput.Log($"{a.Name} wins!", ConsoleColor.Yellow);

            ConsoleOutput.Log("=== BATTLE END ===", ConsoleColor.Yellow);
        }
    }
}
