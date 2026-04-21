import { Component } from "singularity"

export default function App() {
  return Component({
    width: 800,
    height: 600,
    children: [
      Component({ 
        x: 20, 
        y: 30, 
        width: 200, 
        height: 100,
        draw: (ctx) => {
            console.log("draw called");
            ctx.fillRect(0, 0, 100, 50)
        }
      })
    ]
  })
}