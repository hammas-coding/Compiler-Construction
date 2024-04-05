using System;
using System.Linq;
using System.Text;

class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("Welcome to the Password Generator!");
        Console.WriteLine("Please enter your first name:");
        string firstName = Console.ReadLine();

        Console.WriteLine("Please enter your last name:");
        string lastName = Console.ReadLine();

        Console.WriteLine("Please enter your registration numbers:");
        string registrationNumbers = Console.ReadLine();

        string password = GeneratePassword(firstName, lastName, registrationNumbers);
        Console.WriteLine("Generated Password: " + password);
    }

    static string GeneratePassword(string firstName, string lastName, string registrationNumbers)
    {
        StringBuilder password = new StringBuilder();

        password.Append(char.ToUpper(firstName[0]));
        password.Append(char.ToUpper(lastName[0]));

        for (int i = 0; i < firstName.Length; i++)
        {
            if (i % 2 == 0)
                password.Append(firstName[i]);
        }

        for (int i = 0; i < lastName.Length; i++)
        {
            if (i % 2 != 0)
                password.Append(lastName[i]);
        }

        password.Append((char)('A' + new Random().Next(0, 26)));

        var selectedNumbers = registrationNumbers.Where(char.IsDigit).OrderBy(n => Guid.NewGuid()).Take(2);
        foreach (var number in selectedNumbers)
        {
            password.Append(number);
        }

        for (int i = 0; i < 2; i++)
        {
            password.Append(new Random().Next(0, 10));
        }

        string specialChars = "!@#$%^&*()_+-=[]{}|;:,.<>?";
        for (int i = 0; i < 2; i++)
        {
            password.Append(specialChars[new Random().Next(0, specialChars.Length)]);
        }

        string shuffledPassword = new string(password.ToString().OrderBy(x => Guid.NewGuid()).ToArray());

        if (shuffledPassword.Length > 16)
        {
            shuffledPassword = shuffledPassword.Substring(0, 16);
        }

        return shuffledPassword;
    }
}
