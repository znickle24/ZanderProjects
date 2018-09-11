using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ChessTools;
namespace ChessTester
{
    
    class Program
    {
        static void Main(string[] args)
        {
            //still need to grab the 
            List<ChessGame> games = PGNReader.ReadFromFile(filename: @"C:\Users\Zander Nickle\source\repos\znickle\DataSystemsAndApplications\ChessTools\KingBase2018-01.pgn");
            foreach (ChessGame g in games)
                g.Print();

            // Prevent the console from closing after it's done printing
            Console.Read();
        }
    }
}
