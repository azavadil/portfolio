Arbitrage is a program that efficiently finds foreign currency arbitrage opportunities. The code is in arbitrage.py. 

The program uses a queue based Bellman-Ford algorithm to detect negative cycles. Fx rates are converted to logs to enable addition. Please note that the program is not intended for use in actual trading (the program ignores both the bid/ask spread and trading costs).

arbitrage.py pulls live data from Yahoo finance and finds foriegn currency arbitrages. Because the Yahoo data isn't realistic arbritrage.py typically finds circular arbitrage opportunties(e.g. USD->EUR->USD). I've included test cases that demonstrate the more interesting case of finding triangular arbitrage opportunities(e.g. USD->EUR->JPY->USD).

The programs can be run from the command line. To see a live run, download the file and navigate to the directory where the file is saved, and enter the command:

>$python arbitrage.py