package com.example.chapter2;

import java.util.*;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

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
                int idx = leftOps.indexOf(ch); // 如果是开放符号，则压栈
                if (idx >= 0)
                {
                    stack.push((int)ch);
                    continue;
                }
                idx = rightOps.indexOf(ch); // 如果是封闭符号
                if (idx >= 0) {
                    if (stack.isEmpty()) { //栈为空则符号不匹配
                        ok = false;
                        break;
                    } else {
                        int last = stack.pop();
                        int open = leftOps.charAt(idx);
                        if (last != open) { // 弹出的符号不是对应的开放符号
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
