
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace ChessTools {
    public class ChessGame
    {
        private string eventName;
        private string site;
        private string date;
        private string whitePlayer;
        private string blackPlayer;
        private char result;
        private string moves;

        public ChessGame(string _event, string _site, string _date, string _white, string _black, char _result, string _moves)
        {
            eventName = _event;
            site = _site;
            date = _date;
            whitePlayer = _white;
            blackPlayer = _black;
            result = _result;
            moves = _moves;
        }

        public ChessGame()
        {
        }

        public string GetEventName()
        { return eventName; }

        public string GetSite()
        { return site; }

        public string GetDate()
        { return date; }

        public string GetWhitePlayer()
        { return whitePlayer; }

        public string GetBlackPlayer()
        { return blackPlayer; }

        public string GetMoves()
        { return moves; }

        public char GetResult()
        { return result; }

        /// <summary>
        /// Prints all the fields in this ChessGame object,
        /// each on their own line, with a label.
        /// </summary>
        public void Print()
        {
            Console.WriteLine("Event: " + GetEventName());
            Console.WriteLine("Site: " + GetSite());
            Console.WriteLine("Date: " + GetDate());
            Console.WriteLine("White Player: " + GetWhitePlayer());
            Console.WriteLine("Black Player: " + GetBlackPlayer());
            Console.WriteLine("Moves: " + GetMoves());
            Console.WriteLine("Result: " + GetResult());
        }
    }
}