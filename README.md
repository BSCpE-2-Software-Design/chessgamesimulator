# chessgamesimulator
its allows the player to play chess, and it could use int tournaments for constantly having an update to the match being played by the actual player

```mermaid
classDiagram
    %% Attributes Classes
    class PieceType {
        Attributes
        KING
        QUEEN
        ROOK
        BISHOP
        KNIGHT
        PAWN
        NONE
    }

    class Color {
        Attributes
        WHITE
        BLACK
        NONE
    }

    %% Action
    class PieceOption {
        +int row
        +int col
        +string name
        +string position
    }

    class MoveOption {
        +int toRow
        +int toCol
        +string description
    }

    %% Base Character Class
    class Character {
        #string m_name
        #Color m_color
        +Character()
        +Character(string, Color)
        +string getName()
        +Color getColor()
        +void setName(string)
        +void setColor(Color)
        +virtual void displayInfo()
        +virtual string getRole()
        +string getColorString()
    }

    %% Player Class inherits from Character
    class Player {
        -int m_score
        -int m_gamesPlayed
        -int m_gamesWon
        -int m_gamesDrawn
        +Player()
        +Player(string, Color)
        +int getScore()
        +int getGamesPlayed()
        +int getGamesWon()
        +int getGamesDrawn()
        +double getWinRate()
        +void setScore(int)
        +void addWin()
        +void addLoss()
        +void addDraw()
        +void resetStats()
        +void displayInfo()
        +string getRole()
        +void updateScore(int)
    }

    %% Piece Class
    class Piece {
        +PieceType type
        +Color color
        +bool hasMoved
        +Piece(PieceType, Color)
        +char getSymbol()
        +string getName()
        +string getFullName()
    }

    %% ChessBoard Class
    class ChessBoard {
        -vector~vector~Piece~~ m_board
        -Color m_currentTurn
        -bool m_gameOver
        -string m_lastMoveDetail
        -bool m_isInCheck
        -Color m_checkColor
        +ChessBoard()
        +void initializeBoard()
        +void display()
        +string getSquareName(int, int)
        +pair~int,int~ findKing(Color)
        +bool isSquareAttacked(int, int, Color)
        +bool isKingInCheck(Color)
        +bool hasAnyLegalMove(Color)
        +bool wouldMoveCauseCheck(int, int, int, int)
        +bool isValidMove(int, int, int, int)
        +bool isValidMoveWithoutCheck(int, int, int, int, bool, vector~vector~Piece~~)
        +bool isClearPath(int, int, int, int, vector~vector~Piece~~)
        +vector~PieceOption~ getAvailablePieces()
        +void displayPieceMenu()
        +vector~MoveOption~ getAvailableMovesForPiece(int, int)
        +void displayMoveMenuForPiece(int, int)
        +bool executeMove(int, int, int, int)
        +bool selectPieceAndMove()
        +bool isGameOver()
        +void setGameOver(bool)
        +Color getCurrentTurn()
    }

    %% Inheritance Relationships
    Character <|-- Player

    %% Composition Relationships
    ChessBoard *-- Piece : contains 64
    ChessBoard ..> PieceOption : uses
    ChessBoard ..> MoveOption : uses
    ChessBoard --> Color : uses
    Piece --> PieceType : uses
    Piece --> Color : uses
    Player --> Color : uses
    Character --> Color : uses

    %% Dependency Relationships
    ChessBoard ..> Player : interacts with
    main ..> ChessBoard : creates
    main ..> Player : creates
    main ..> Character : polymorphic usage
```
CORE GAME PLAY
```mermaid
sequenceDiagram
    participant User
    participant Main
    participant Board
    participant Piece
    
    User->>Main: Start game
    Main->>Board: new ChessBoard()
    Board->>Board: initializeBoard()
    
    loop Until Checkmate/Stalemate
        Board->>User: display board
        
        loop Until valid move
            User->>Board: select piece
            Board->>Board: getAvailableMoves()
            Board->>User: show moves
            User->>Board: select move
            Board->>Piece: isValidMove()
            Piece-->>Board: valid/invalid
        end
        
        Board->>Board: executeMove()
        Board->>Board: switchTurn()
        Board->>Board: isKingInCheck()
        
        alt Game Over
            Board-->>Main: gameOver = true
        end
    end
    
    Main->>User: display winner
```
MOVE VILIDATION 
```mermaid
sequenceDiagram
    participant User
    participant Board
    participant Piece
    participant King
    
    User->>Board: selectMove(fromRow, fromCol, toRow, toCol)
    
    Board->>Piece: isValidMoveWithoutCheck()
    
    alt Basic validation fails
        Piece-->>Board: return false
        Board->>User: Invalid move
    else Basic validation passes
        Piece-->>Board: return true
        Board->>Board: wouldMoveCauseCheck()
        
        Board->>Board: Create temporary board
        Board->>Board: Simulate move on temp board
        Board->>King: Find king position
        King-->>Board: Return king location
        Board->>Board: isSquareAttacked(king position)
        
        alt King would be in check
            Board->>User: Move would put king in check
            Board-->>User: return false
        else King safe
            Board->>Board: isValidMove = true
            Board->>Piece: Execute move
            Board->>User: Move successful
            Board-->>User: return true
        end
    end
```
PLAYER TURN MANAGEMENT
```mermaid
sequenceDiagram
    participant Board
    participant WhitePlayer
    participant BlackPlayer
    
    Board->>Board: currentTurn = WHITE
    Board->>WhitePlayer: displayInfo()
    
    loop White's turn
        WhitePlayer->>Board: selectPieceAndMove()
        Board->>Board: executeMove()
        
        alt Move valid
            Board->>Board: Switch turn
            Board->>Board: currentTurn = BLACK
            Board-->>WhitePlayer: Turn ended
        else Move invalid
            Board->>WhitePlayer: Try again
        end
    end
    
    Board->>BlackPlayer: displayInfo()
    
    loop Black's turn
        BlackPlayer->>Board: selectPieceAndMove()
        Board->>Board: executeMove()
        
        alt Move valid
            Board->>Board: Switch turn
            Board->>Board: currentTurn = WHITE
            Board-->>BlackPlayer: Turn ended
        else Move invalid
            Board->>BlackPlayer: Try again
        end
    end
```
CHECK AND CHECK MATE DETECTION
```mermaid
sequenceDiagram
    participant Board
    participant King
    participant Opponent
    participant Move
    
    Board->>Board: executeMove()
    Board->>Board: switchTurn()
    Board->>King: findKing(currentTurn)
    King-->>Board: Return king position
    
    Board->>Opponent: Loop through all opponent pieces
    loop For each opponent piece
        Opponent->>Board: isValidMoveWithoutCheck()
        alt Can attack king
            Board->>Board: isInCheck = true
            Board->>Board: Set checkColor
            Board->>User: Display "CHECK!"
        end
    end
    
    alt isInCheck == true
        Board->>Move: Find all legal moves
        Move-->>Board: Return moves list
        
        alt No legal moves available
            Board->>Board: gameOver = true
            Board->>User: Display "CHECKMATE!"
        else Legal moves available
            Board->>User: Must move out of check
        end
    end
```
