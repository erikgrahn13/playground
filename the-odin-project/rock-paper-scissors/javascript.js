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

  let round = (humanChoice, computerChoice) => {
    humanChoice = humanChoice.toLowerCase(humanChoice);

    if (
      (humanChoice == "rock" && computerChoice == "scissor") ||
      (humanChoice == "paper" && computerChoice == "rock") ||
      (humanChoice == "scissor" && computerChoice == "paper")
    ) {
      console.log(`You win! ${humanChoice} beats ${computerChoice}`);
      humanScore++;
    } else if (humanChoice == computerChoice) {
      console.log(`Draw! ${humanChoice} is the same as ${computerChoice}`);
    } else {
      console.log(`You lose! ${computerChoice} beats ${humanChoice}`);
      computerScore++;
    }

    console.log(
      `Current score: You: ${humanScore}   Computer: ${computerScore}`
    );
  };

  for (let i = 0; i < 5; i++) {
    const humanSelection = getHumanChoice();
    const computerSelection = getComputerChoice();
    round(humanSelection, computerSelection);
  }

  if (humanScore > computerScore) {
    console.log("YOU WON! CONGRATULATIONS");
  } else if (humanScore === computerScore) {
    console.log("DRAW!");
  } else {
    console.log("YOU LOST! BETTER LUCK NEXT TIME");
  }
}

playGame();
