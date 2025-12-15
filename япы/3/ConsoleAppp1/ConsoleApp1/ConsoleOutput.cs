using System;
using System.IO;

namespace AutoBattler
{
    public static class ConsoleOutput
    {
        private static readonly string LogFile = Path.Combine(Environment.CurrentDirectory, "battle_log.txt");

        public static void Log(string text, ConsoleColor color)
        {
            Console.ForegroundColor = color;
            Console.WriteLine(text);
            Console.ResetColor();

            try
            {
                File.AppendAllText(LogFile, $"[{DateTime.Now:yyyy-MM-dd HH:mm:ss}] {text}{Environment.NewLine}");
            }
            catch
            {
                // ignore file errors
            }
        }
    }
}
