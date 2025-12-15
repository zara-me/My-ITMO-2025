using System;
using System.IO;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

class Program
{
    static async Task Main(string[] args)
    {
        string folder = args.Length > 0 ? args[0] : Directory.GetCurrentDirectory();

        if (!Directory.Exists(folder))
        {
            Console.WriteLine($"Folder not found: {folder}");
            return;
        }

        //txt , csv — and Distinct for Removes duplicate paths
        var files = Directory.GetFiles(folder, "*.*")
                             .Where(f => f.EndsWith(".txt", StringComparison.OrdinalIgnoreCase)
                                      || f.EndsWith(".csv", StringComparison.OrdinalIgnoreCase))
                             .Distinct()
                             .ToArray();

        if (files.Length == 0)
        {
            Console.WriteLine("No .txt or .csv files found in folder.");
            return;
        }

        var stopWatch = Stopwatch.StartNew();
        Console.WriteLine($"Found {files.Length} files. Starting word count...");
        //все файлы одновременно, а не по одному :)
        var tasks = new List<Task<(string file, int count)>>();
        foreach (var f in files)
        {
            tasks.Add(CountWordsAsync(f));
        }
        //параллельно
        await foreach (var finished in Task.WhenEach(tasks))
        {
            var (file, count) = await finished;
            Console.WriteLine($"{Path.GetFileName(file)} => {count} words  (elapsed: {stopWatch.Elapsed})");
        }
        //totalTime
        stopWatch.Stop();
        Console.WriteLine($"Total elapsed time: {stopWatch.Elapsed}");
    }

    static async Task<(string file, int count)> CountWordsAsync(string filePath)
    {
        string text = await File.ReadAllTextAsync(filePath);
        int count = await Task.Run(() => CountWords(text));
        return (filePath, count);
    }

    static int CountWords(string text)
    {
        if (string.IsNullOrWhiteSpace(text)) return 0;
        var matches = Regex.Matches(text, @"\p{L}+|\d+");
        return matches.Count;
    }
}