package com.example.chapter2;

import java.util.*;


public class ReversePolish {
    public static String operators = ("+-*/^");
    public static Map<String, Integer> precedence = new HashMap<>();
    static {
        precedence.put("+", 1);
        precedence.put("-", 1);
        precedence.put("*", 2);
        precedence.put("/", 2);
        precedence.put("^", 3);
    }

    private static String calculate(String operator, String op1, String op2) {
        double f1 = Double.parseDouble(op1);
        double f2 = Double.parseDouble(op2);
        switch (operator) {
            case "+": return (f1 + f2) + "";
            case "-": return (f1 - f2) + "";
            case "*": return (f1 * f2) + "";
            case "/": return (f1 / f2) + "";
            case "^": return Math.pow(f1, f2) + "";
        }
        return "";
    }

    // 计算后缀表达式
    // 6 5 2 3 + 8 * + 3 + *  ==>
    private static double evaluatePostfix(String expression) {
        ResizingArrayStack<String> stack = new ResizingArrayStack<>();
        Scanner scanner = new Scanner(expression);
        while (scanner.hasNext()) {
            String s = scanner.next();
            if (operators.contains(s)) {      //是操作符则弹出2个操作数进行计算
                String op1 = stack.pop();
                String op2 = stack.pop();
                String result = calculate(s, op1, op2);
                stack.push(result);
            } else {        // 若是操作数直接入栈
                stack.push(s);
            }
        }
        return Double.parseDouble(stack.pop());
    }

    private static String infixToPostfix(String expression) {
        StringBuilder sb = new StringBuilder();
        ResizingArrayStack<String> stack = new ResizingArrayStack<>();
        Scanner scanner = new Scanner(expression);
        while (scanner.hasNext()) {
            String s = scanner.next();
            if (s.equals("(")) {
                stack.push(s);
            } else if (s.equals(")")) { // 遇到右括号将栈弹出，直至弹出对应的左括号
                while (!stack.isEmpty()) {
                    String op = stack.pop();
                    if (op.equals("("))
                        break;
                    sb.append(op);
                    sb.append(" ");
                }
            } else {
                int idx = operators.indexOf(s); // 是操作符入栈，先弹出比自己优先级高的操作符
                if (idx >= 0) {
                    while (!stack.isEmpty()) {
                        String top = stack.peek();
                        if (top.equals("("))
                            break;
                        int i = precedence.get(s);
                        int j = precedence.get(top);
                        if (j >= i) { // 优先级更高
                            stack.pop();
                            sb.append(top);
                            sb.append(" ");
                        } else {
                            break;
                        }
                    }
                    stack.push(s);
                } else {
                    // 是操作数则放入输出
                    sb.append(s);
                    sb.append(" ");
                }
            }
        }
        while (!stack.isEmpty()) {
            String s = stack.pop();
            sb.append(s);
            sb.append(" ");
        }
        return sb.toString().trim();
    }

    private static String postfixToInfix(String expression)
    {
        ResizingArrayStack<String> stack = new ResizingArrayStack<>();
        Scanner scanner = new Scanner(expression);
        while (scanner.hasNext()) {
            String s = scanner.next();
            int idx = operators.indexOf(s);
            if (idx >= 0) {
                // this is a operator
                assert stack.size() >= 2;
                String right = stack.pop();
                String left = stack.pop();
                String result = String.format("( %s %s %s )", left, s, right);
                stack.push(result);
            } else {
                stack.push(s);
            }
        }
        assert stack.size() == 1;
        return stack.pop();
    }

    public static void main(String[] args) {
        String[] data1 = new String[]{
                "a + b * c + ( d * e + f ) * g",
                "A + ( B * C - ( D / E ^ F ) * G ) * H",
        };
        String[] data2 = new String[]{
                "a b c * + d e * f + g * +",
                "A B C * D E F ^ / G * - H * + ",
        };
        for (int i = 0; i < data1.length; i++) {
            String input = data1[i].trim();
            String output1 = infixToPostfix(input);
            String output2 = postfixToInfix(data2[i]);
            String expected = data2[i].trim();
            if (!output1.equals(expected)) {
                System.out.println(String.format("not equal, %s, %s", output1, expected));
            }
            System.out.println(input + " equals " + output2);
        }


        String expression = "6 * ( 5 + ( 2 + 3 ) * 8 ) + 3";
        String postfixExpr = infixToPostfix(expression);
        double result = evaluatePostfix(postfixExpr);
        System.out.println(expression + " equals " + result);
    }
}
