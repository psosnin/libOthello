import sys
sys.path.insert(0, '/home/phil/Coding/Othello/LibOthello/lib')

from pythello import GameEngine

game = GameEngine()

while not game.gameEnded():
    game.print()
    valid_moves = game.getMoveList()
    print("Black to move, valid moves:\n\t", end = "")
    for i in valid_moves:
        print(i, end=' ')
    move = int(input("\nEnter move: "))
    while game.playMove(move) == -3:
        print("Not a valid move")
        move = input("Enter move: ")
    
    game.print()
    valid_moves = game.getMoveList()
    print("White to move, valid moves:\n\t", end = "")
    for i in valid_moves:
        print(i, end=' ')
    move = int(input("\nEnter move: "))
    while game.playMove(move) == -3:
        print("Not a valid move")
        move = input("Enter move: ")

print("Final score, Black = {}".format(game.getScore()))
    