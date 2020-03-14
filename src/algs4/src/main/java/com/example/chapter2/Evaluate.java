package com.example.chapter2;

import java.util.Stack;
import edu.princeton.cs.algs4.*;


class Evaluate
{
    public static void main(String[] args)
    {
        Stack<String> ops = new Stack<>();
        Stack<Double> vals = new Stack<>();
        StdOut.println("Enter expression: ");
        while (!StdIn.isEmpty())
        {
            String s = StdIn.readString();
            if (s.equals("(")) {
                // do nothing
            } else if (s.equals("+") || s.equals("-") || s.equals("*") || s.equals("/")) {
                ops.push(s);
            } else if (s.equals(")")) {
                String op = ops.pop();
                double v = vals.pop();
                switch (op) {
                    case "+": v += vals.pop(); break;
                    case "-": v = vals.pop() - v; break;
                    case "*": v *= vals.pop(); break;
                    case "/": v = vals.pop() / v; break;
                }
                vals.push(v);
            } else {
                vals.push(Double.parseDouble(s));
            }
        }
        StdOut.println(vals.pop());
    }
}