function getComputerChoice() {
  const randomNumber = Math.floor(Math.random() * 3);

  let string;
  switch (randomNumber) {
    case 0:
      string = "rock";
      break;
    case 1:
      string = "paper";
      break;
    case 2:
      string = "scissor";
      break;
    default:
  }

  return string;
}

function getHumanChoice() {
  let humanChoice = prompt("Chose rock , paper or scissor");

  return humanChoice;
}

function playGame() {
  let humanScore = 0;
  let computerScore = 0;

  let result = document.createElement("div");
  let round = (humanChoice, computerChoice) => {
    humanChoice = humanChoice.toLowerCase(humanChoice);

    if (
      (humanChoice == "rock" && computerChoice == "scissor") ||
      (humanChoice == "paper" && computerChoice == "rock") ||
      (humanChoice == "scissor" && computerChoice == "paper")
    ) {
      console.log(`You win! ${humanChoice} beats ${computerChoice}`);
      result.textContent = `You win! ${humanChoice} beats ${computerChoice}`;
      humanScore++;
    } else if (humanChoice == computerChoice) {
      console.log(`Draw! ${humanChoice} is the same as ${computerChoice}`);
      result.textContent = `Draw! ${humanChoice} is the same as ${computerChoice}`;
    } else {
      console.log(`You lose! ${computerChoice} beats ${humanChoice}`);
      result.textContent = `You lose! ${computerChoice} beats ${humanChoice}`;
      computerScore++;
    }

    console.log(
      `Current score: You: ${humanScore}   Computer: ${computerScore}`
    );
    result.textContent += `
      Current score: You: ${humanScore}   Computer: ${computerScore}`;
    
    checkWin();
  };


  function checkWin() {
    if (humanScore === 5) {
      console.log("YOU WON! CONGRATULATIONS");
      result.textContent = "YOU WON! CONGRATULATIONS";
    } else if (computerScore === 5) {
      console.log("YOU LOST! BETTER LUCK NEXT TIME");
      result.textContent = "YOU LOST! BETTER LUCK NEXT TIME"
    }
  }
  let rockButton = document.createElement("button");
  rockButton.textContent = "Rock";
  rockButton.addEventListener("click", () => {
    const humanSelection = rockButton.textContent;
    const computerSelection = getComputerChoice();
    round(humanSelection, computerSelection);
  });

  let paperButton = document.createElement("button");
  paperButton.textContent = "Paper";
  paperButton.addEventListener("click", () => {
    const humanSelection = paperButton.textContent;
    const computerSelection = getComputerChoice();
    round(humanSelection, computerSelection);
  });

  let scissorButton = document.createElement("button");
  scissorButton.textContent = "Scissor";
  scissorButton.addEventListener("click", () => {
    const humanSelection = scissorButton.textContent;
    round(scissorButton.textContent, getComputerChoice());
  });

  let body = document.querySelector("body");
  body.appendChild(rockButton);
  body.appendChild(paperButton);
  body.appendChild(scissorButton);
  body.appendChild(result);
}



  // for (let i = 0; i < 5; i++) {
  //   const humanSelection = getHumanChoice();
  //   const computerSelection = getComputerChoice();
  //   round(humanSelection, computerSelection);
  // }


playGame();
