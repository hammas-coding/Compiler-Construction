using System;
using System.Collections.Generic;

namespace SimpleGrammar
{
    class Program
    {
        static Dictionary<char, string> grammar = new Dictionary<char, string>
        {
            {'A', "aB"},
            {'B', "bC"},
            {'C', "cD"},
            {'D', "d"}
        };

        static void Main(string[] args)
        {
            Console.WriteLine("Grammar Productions:");
            foreach (var rule in grammar)
            {
                Console.WriteLine($"{rule.Key} -> {rule.Value}");
            }

            Console.WriteLine("\nEnter a non-terminal to see its production (A, B, C, D): ");
            char nonTerminal = Console.ReadKey().KeyChar;
            Console.WriteLine();

            if (grammar.ContainsKey(nonTerminal))
            {
                string production = grammar[nonTerminal];
                Console.WriteLine($"Production for {nonTerminal}: {nonTerminal} -> {production}");
            }
            else
            {
                Console.WriteLine("Invalid non-terminal. Please enter one of A, B, C, D.");
            }
        }
    }
}
