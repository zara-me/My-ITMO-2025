using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Linq;

[StructLayout(LayoutKind.Sequential)]
public struct Point
{
    public int x;
    public int y;
}

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate int PointPredicate(Point p);

class Program
{
    [DllImport("point_lib.dll", CallingConvention = CallingConvention.Cdecl)]
    static extern int filter([In] Point[] input, int count, [Out] Point[] output, PointPredicate pred);

    // optional helpers from your DLL (keep if you want to test)
    [DllImport("point_lib.dll", CallingConvention = CallingConvention.Cdecl)]
    static extern void process_point(ref Point p);

    [DllImport("point_lib.dll", CallingConvention = CallingConvention.Cdecl)]
    static extern void procces_array_point([In, Out] Point[] p, int count);

    static Point[] GenerateRandomPoints(int n, int range = 10)
    {
        var rnd = new Random(0); // ثابت بودن seed -> reproducible
        var arr = new Point[n];
        for (int i = 0; i < n; ++i)
        {
            arr[i].x = rnd.Next(-range, range + 1);
            arr[i].y = rnd.Next(-range, range + 1);
        }
        return arr;
    }

    static void SavePointsBinary(string path, Point[] pts)
    {
        using var bw = new BinaryWriter(File.Open(path, FileMode.Create, FileAccess.Write));
        bw.Write(pts.Length);
        foreach (var p in pts)
        {
            bw.Write(p.x);
            bw.Write(p.y);
        }
    }

    static Point[] LoadPointsBinary(string path)
    {
        using var br = new BinaryReader(File.Open(path, FileMode.Open, FileAccess.Read));
        int n = br.ReadInt32();
        var res = new Point[n];
        for (int i = 0; i < n; ++i)
        {
            res[i].x = br.ReadInt32();
            res[i].y = br.ReadInt32();
        }
        return res;
    }

    // predicates
    static int IsQuadrant1(Point p) => (p.x > 0 && p.y > 0) ? 1 : 0;
    static int IsQuadrant2(Point p) => (p.x < 0 && p.y > 0) ? 1 : 0;
    static int IsQuadrant3(Point p) => (p.x < 0 && p.y < 0) ? 1 : 0;
    static int IsQuadrant4(Point p) => (p.x > 0 && p.y < 0) ? 1 : 0;

    static void SaveCsv(string filename, Point[] pts, int count)
    {
        using var sw = new StreamWriter(filename);
        sw.WriteLine("x,y");
        for (int i = 0; i < count; ++i)
            sw.WriteLine($"{pts[i].x},{pts[i].y}");
    }

    static void PrintSample(string title, Point[] pts, int count, int maxShow = 10)
    {
        Console.WriteLine($"{title} count = {count}");
        for (int i = 0; i < Math.Min(count, maxShow); ++i)
            Console.WriteLine($"  {i+1}: x={pts[i].x} y={pts[i].y}");
        if (count > maxShow) Console.WriteLine($"  ... (and {count - maxShow} more)");
    }

    static void Main()
    {
        string path = "points.bin";
        if (!File.Exists(path))
        {
            var pts = GenerateRandomPoints(50, 10);
            SavePointsBinary(path, pts);
            Console.WriteLine("Generated and saved points.bin");
        }

        var input = LoadPointsBinary(path);
        Console.WriteLine($"Loaded {input.Length} points.");

        var q1 = new Point[input.Length];
        var q2 = new Point[input.Length];
        var q3 = new Point[input.Length];
        var q4 = new Point[input.Length];
 
        int c1 = filter(input, input.Length, q1, IsQuadrant1);
        int c2 = filter(input, input.Length, q2, IsQuadrant2);
        int c3 = filter(input, input.Length, q3, IsQuadrant3);
        int c4 = filter(input, input.Length, q4, IsQuadrant4);

        Console.WriteLine();
        PrintSample("Quadrant1", q1, c1);
        PrintSample("Quadrant2", q2, c2);
        PrintSample("Quadrant3", q3, c3);
        PrintSample("Quadrant4", q4, c4);

        // save CSVs
        SaveCsv("q1.csv", q1, c1);
        SaveCsv("q2.csv", q2, c2);
        SaveCsv("q3.csv", q3, c3);
        SaveCsv("q4.csv", q4, c4);
        Console.WriteLine();
        Console.WriteLine("CSV files saved: q1.csv, q2.csv, q3.csv, q4.csv");
    }
}
