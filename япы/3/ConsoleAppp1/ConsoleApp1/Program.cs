using System;

namespace AutoBattler
{
    class Program
    {
        static void Main(string[] args)
        {
            var rand = new Random();

            Hero hero1 = new Warrior("Conan", rand.Next(50, 101), rand.Next(0, 10), rand.Next(5, 16));
            Hero hero2 = new Wizzard("Merlin", rand.Next(40, 91), rand.Next(0, 8), rand.Next(6, 14));

            ConsoleOutput.Log("Initialized heroes:", ConsoleColor.White);
            ConsoleOutput.Log(hero1.ToString(), ConsoleColor.DarkRed);
            ConsoleOutput.Log(hero2.ToString(), ConsoleColor.DarkMagenta);

            var battler = new AutoBattler(hero1, hero2);
            battler.StartBattle();

            ConsoleOutput.Log("Press any key to exit...", ConsoleColor.Gray);
            Console.ReadKey();
        }
    }
}
