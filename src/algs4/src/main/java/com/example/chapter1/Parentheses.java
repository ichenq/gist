package com.example.chapter1;

import java.util.*;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

// Exercise 1.3.4
public class Parentheses {
    public static String leftOps = new String("[({");
    public static String rightOps = new String("])}");

    // test cases: [()]{}{[()()]()}  [(])
    public static void main(String[] args) {
        ResizingArrayStack<Integer> stack = new ResizingArrayStack();
        while (!StdIn.isEmpty()) {
            String s = StdIn.readString();
            boolean ok = true;
            for (int i = 0; i < s.length(); i++) {
                char ch = s.charAt(i);
                if (leftOps.contains(s)) // 如果是开放符号，则压栈
                {
                    stack.push((int)ch);
                    continue;
                }
                int idx = rightOps.indexOf(ch); // 如果是封闭符号
                if (idx >= 0) {
                    if (stack.isEmpty()) { //栈为空则符号不匹配
                        ok = false;
                        break;
                    } else {
                        int last = stack.pop(); // 判断弹出的符号是不是对应的开放符号
                        int open = leftOps.charAt(idx);
                        if (last != open) {
                            ok = false;
                            break;
                        }
                    }
                }
            }
            //扫描完，如果栈非空，则符号不匹配
            if (!stack.isEmpty()) {
                ok = false;
            }
            StdOut.println(ok);
        }
    }
}
