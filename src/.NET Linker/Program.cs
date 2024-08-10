using System.Diagnostics;
using Jvav.Linker;

namespace Jvav;

internal class Program
{
    public static void Main(string[] args)
    {
        foreach (var arg in args)
        {
            Console.WriteLine($"linking {Path.GetFileName(arg)}");
            Linking(arg);
        }

        if (Debugger.IsAttached)
        {
            while (true)
            {
                var input = Console.ReadLine();
                if (string.IsNullOrWhiteSpace(input))
                {
                    return;
                }

                Linking(input);
            }
        }
    }

    private static void Linking(string inputFile)
    {
        try
        {
            using var inputStream = File.OpenRead(inputFile);
            var linker = new LinkingReader(inputStream);

            var linkerVersion = linker.ReadLinkerVersion();
            switch (linkerVersion)
            {
                case 1:
                    LinkingV1(linker);
                    break;
                default:
                    Console.WriteLine($"error: unsupported linker version {linkerVersion}");
                    break;
            }
        }
        catch (Exception exception)
        {
            try
            {
                Console.WriteLine($"error: failed to open file '{Path.GetFileName(inputFile)}', error:\n - {exception.Message}");
            }
            catch (Exception innerException)
            {
                Console.WriteLine($"error: failed to open file '{inputFile}', errors:\n - {exception.Message}\n - {innerException.Message}");
            }
        }
    }

    private static void LinkingV1(LinkingReader reader)
    {
        try
        {
            var linker = reader.LinkV1();
            Console.WriteLine($"linking {linker.ModuleName} with linker version 1");

            linker.Export();
        }
        catch (Exception exception)
        {
            Console.WriteLine($"error: failed to read linking data, error:\n - {exception}");
        }
    }
}