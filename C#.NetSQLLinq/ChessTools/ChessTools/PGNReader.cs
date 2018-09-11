using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessTools { 
    public static class PGNReader
    {
        private static string parseString(string _toParse)
        {
            string[] parsed = _toParse.Split('"');
            return parsed[1];
        }
        /// <summary>
        /// Reads a text file containing one or more PGN format chess games
        /// and extracts the Event, Site, Date, player names, result, and moves
        /// of each game.
        /// </summary>
        /// <param name="filename">The full path to the file containing PGN data</param>
        /// <returns>A list of ChessGame, one for each in the input file</returns>
        public static List<ChessGame> ReadFromFile(string filename)
        {
            string[] fileContents = System.IO.File.ReadAllLines(filename);
            
            List<ChessGame> retval = new List<ChessGame>();
            string _eventNombre = "";
            string _site = "";
            string _date = "";
            string _whitePlayer = "";
            string _blackPlayer = "";
            char _result = 'b';
            string _moves = "";
            int spaces = 0; 
            for (int i = 0; i < fileContents.Length; i++)
            {
                if (fileContents[i].StartsWith("[Event "))
                {
                    _eventNombre = parseString(fileContents[i]);
                } else if (fileContents[i].StartsWith("[Si"))
                {
                    _site = parseString(fileContents[i]);
                } else if (fileContents[i].StartsWith("[Date "))
                {
                    _date = parseString(fileContents[i]);
                } else if (fileContents[i].StartsWith("[White "))
                {
                    _whitePlayer = parseString(fileContents[i]);
                } else if (fileContents[i].StartsWith("[Black "))
                {
                    _blackPlayer = parseString(fileContents[i]);
                } else if (fileContents[i].StartsWith("[Res"))
                {
                    string parseQuotesFromResult = parseString(fileContents[i]);

                    if (parseQuotesFromResult == "0-1")
                    {
                        _result = 'b';
                    }
                    else if (parseQuotesFromResult == "1-0")
                    {
                        _result = 'w';
                    }
                    else
                    {
                        _result = 'd';
                    }
                } else if (string.IsNullOrEmpty(fileContents[i]))
                {
                    i++;
                    int j = i;
              
                    while ((!string.IsNullOrEmpty(fileContents[j]))) {
                        _moves += fileContents[j];
                        j++;
                    }
                    
                    spaces++;
                    i = j; 
                    //create new game object
                    ChessGame nextGame = new ChessGame(_eventNombre, _site, _date, _whitePlayer, _blackPlayer, _result, _moves);
                    _moves = "";
                    //add game object to list of game objects
                    retval.Add(nextGame);
                }
                
            }
            // TODO: Don't return an empty list
            return retval;
        }
        }
}