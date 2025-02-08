function add(a, b) {
  return a + b;
}

function subtract(a, b) {
  return a - b;
}

function multiply(a, b) {
  return a * b;
}

function divide(a, b) {
  if (b === 0) {
    operandA = null;
    operandB = null;
    operator = null;
    return "LOSER! Don't divide by zero!";
  }
  return a / b;
}

function operate(a, b, operator) {
  let answer;
  a = Number(a);
  b = Number(b);
  switch (operator) {
    case "+":
      answer = add(a, b);
      break;
    case "-":
      answer = subtract(a, b);
      break;
    case "*":
      answer = multiply(a, b);
      break;
    case "/":
      answer = divide(a, b);
      break;
    default:
      alert("Unsupported operation");
      break;
  }

  return answer;
}

function handleNumbers(number) {
  if (!operator) {
    if (!operandA) {
      operandA = number;
      display.textContent = number;
    } else {
      operandA += number;
      display.textContent += number;
    }
    newUserInput = true;
  } else {
    if (!operandB) {
      operandB = number;
      display.textContent = number;
    } else {
      operandB += number;
      display.textContent += number;
    }
    newUserInput = true;
  }
}

function handleOperators(op) {
  if (operator) {
    if (operandA && operandB) {
      answer = operate(operandA, operandB, operator);
      display.textContent = answer;
      operandA = answer;
    }
    operator = op;
    operandB = null;
  } else {
    operator = op;
  }
}

function handleDecimal() {
  if (!operator) {
    if (!operandA) {
      operandA = "0";
    }

    if (String(operandA).indexOf(".") < 0) {
      operandA += ".";
      display.textContent = operandA;
      newUserInput = true;
    }
  } else {
    if (!operandB) {
      operandB = "0";
    }
    if (String(operandB).indexOf(".") < 0) {
      operandB += ".";
      display.textContent = operandB;
      newUserInput = true;
    }
  }
}

function handleEqual() {
  if (operandA && operandB) {
    let answer = operate(Number(operandA), Number(operandB), operator);
    display.textContent = answer;
    operandA = answer;
    operandB = null;
    operator = null;
    newUserInput = false;
  }
}

function handleBackspace() {
  if (newUserInput) {
    if (!operator) {
      if (operandA) {
        let tmp = String(operandA).split("");
        tmp.splice(operandA.length - 1, 1);
        operandA = tmp.join("");
        if (operandA.length === 0) {
          operandA = null;
          display.textContent = "0";
        } else {
          display.textContent = operandA;
        }
      }
    } else {
      if (operandB) {
        let tmp = String(operandB).split("");
        tmp.splice(operandB.length - 1, 1);
        operandB = tmp.join("");
        if (operandB.length === 0) {
          operandB = null;
          display.textContent = "0";
        } else {
          display.textContent = operandB;
        }
      }
    }
  }
}

function handleClear() {
  operandA = null;
  operandB = null;
  operator = null;
  display.textContent = "0";
}

let numbers = document.querySelectorAll(".numbers");
let display = document.querySelector(".display");
let operators = document.querySelectorAll(".operators");
let equal = document.querySelector(".equal");
let buttons = document.querySelectorAll("button");

document.addEventListener("keydown", (event) => {
  if (event.key >= "0" && event.key <= "9") {
    handleNumbers(event.key);
  } else if (event.key == ".") {
    handleDecimal();
  } else if (
    event.key == "+" ||
    event.key == "-" ||
    event.key == "*" ||
    event.key == "/"
  ) {
    handleOperators(event.key);
  } else if (event.key == "=" || event.key == "Enter") {
    handleEqual();
  } else if (event.key == "Backspace") {
    handleBackspace();
  } else if (event.key == "Escape") {
    handleClear();
  }
});

buttons.forEach((button) => {
  button.addEventListener("mouseover", (event) => {
    button.style.backgroundColor = "orchid";
  });
  button.addEventListener("mouseout", (event) => {
    button.style.backgroundColor = "darkorchid";
  });
});

equal.addEventListener("click", (event) => {
  handleEqual();
});

operators.forEach((op) => {
  op.addEventListener("click", (event) => {
    handleOperators(op.textContent);
  });
});

let clearButton = document.querySelector(".clear");

clearButton.addEventListener("click", (event) => {
  handleClear();
});

numbers.forEach((number) => {
  number.addEventListener("click", (event) => {
    handleNumbers(number.textContent);
  });
});

let decimalButton = document.querySelector(".decimal");
decimalButton.addEventListener("click", (event) => {
  handleDecimal();
});

let backspaceButton = document.querySelector(".backspace");
backspaceButton.addEventListener("click", (event) => {
  handleBackspace();
});

let operandA;
let operandB;
let operator;
let decimal = true;
let newUserInput = true;
