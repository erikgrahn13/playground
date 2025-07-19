

const Gameboard = (function() {
    let gameboard = [".", ".", ".", ".", ".", ".",".", ".", "."]
    function getGameboard() { return gameboard; }
    return { getGameboard }
})();


function createPlayer(name) {
    return {
        name: name
    }
}

let button = document.querySelector("button");
let player1 = document.querySelector(".player1");
let player2 = document.querySelector(".player2");

button.addEventListener("click", () => {

    if(player1.value === "" || player2.value === "") {
        alert("Player names cannot be empty!");
        return;
    }

    let playerOne = createPlayer(player1.value);
    let playerTwo = createPlayer(player2.value);

    let player1Label = document.querySelector("#player1-label");
    player1Label.textContent = `${playerOne.name}: X`;

    let player2Label = document.querySelector("#player2-label");
    player2Label.textContent = `${playerTwo.name}: O`;


    button.textContent = "Restart"
    

    gameController.startGame(playerOne, playerTwo);
})


const gameController = (function() {
    let playerOne;
    let playerTwo;

    let gameOver = false;
    let playerTurn = true;

        let DOMlogic = {

         renderBoard() {
             board = document.querySelector("#board");
            Gameboard.getGameboard().forEach((e, rowIndex) => {
                let element = document.createElement("div");
                element.setAttribute("class", "element")
                board.appendChild(element);
        })

            
        },

        clearBoard() {
            let elements = document.querySelectorAll(".element");
            elements.forEach((e, index) => {
                e.textContent = "";
            })
        }
    }



    function createGame() {
        DOMlogic.renderBoard();
        let elements = document.querySelectorAll(".element");
        elements.forEach((element, elementIndex) => {
                element.addEventListener("click", () => {
                    if(!gameOver && playerOne && playerTwo) {
                        placePiece(elementIndex);
                        element.textContent = Gameboard.getGameboard()[elementIndex];
                    }
                })
            })
    }

    function placePiece(index) {
        if(Gameboard.getGameboard()[index] !== ".")
            return;
        Gameboard.getGameboard()[index] =  playerTurn ? "X" : "O";

        if(checkWin()) {
            if(playerTurn) {
                alert(`Congratulations! ${playerOne.name}`)
                button.textContent = "Start";
            }
            else {
                alert(`Congratulations! ${playerTwo.name}`)
                button.textContent = "Start";
            }
            return;
        }
        else if(gameOver) {
            alert(`TIE!`)
            button.textContent = "Start";
        }
        playerTurn = !playerTurn;

        if(playerTurn) {
            let player1Label = document.querySelector("#player1-label");
            player1Label.style.border = "3px solid green";

            let player2Label = document.querySelector("#player2-label");
            player2Label.style.border = "";
        }
        else {
            let player1Label = document.querySelector("#player1-label");
            player1Label.style.border = "";

            let player2Label = document.querySelector("#player2-label");
            player2Label.style.border = "3px solid green";
        }
    }

    function startGame(player1, player2) {
        console.log("Game started");
        gameOver = false;
        Gameboard.getGameboard().forEach((e, index, board) => {
            board[index] = ".";
        })
        DOMlogic.clearBoard();
        playerTurn = true;

        playerOne = player1;
        playerTwo = player2;

        let player1Label = document.querySelector("#player1-label");
        player1Label.style.border = "3px solid green";
        let player2Label = document.querySelector("#player2-label");
        player2Label.style.border = "";
     }

    function checkWin () {
        
        let board = Gameboard.getGameboard();

        for(let i = 0; i < 9; i+=3) {
            if(board[i] !== "." && board[i] === board[i+1] && board[i+1] === board[i+2]) {
                gameOver = true;
                return true;
            }
        }

        for(let i = 0; i < 9; ++i) {
            if(board[i] !== "." && board[i] === board[i+3] && board[i+3] == board[i+6]){
                gameOver = true;
                return true;
            }
        }

        if(board[0] !== "." && board[0] === board[4] && board[4] === board[8]) {
            gameOver = true;
            return true;
        }

        if(board[6] !== "." && board[6] === board[4] && board[4] === board[2]) {
            gameOver = true;
            return true;
        }

        return false;
    }

    return { createGame, startGame, placePiece}

})();

gameController.createGame();
