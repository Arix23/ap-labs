package main

import (
	"fmt"
	"os"
)

func main() {
  if(len(os.Args)<2){
    fmt.Println("Error, must pass a name as argument")
  } else {
    i := 1
    name := ""
    for i<len(os.Args){
      if i==1 {
        name = name + os.Args[i]
      } else{
        name = name + " " + os.Args[i]
      }
      i=i+1
    }
    fullString := "Hello " + name + ", Welcome to the Jungle"
	  fmt.Println(fullString)
  }
}