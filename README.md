# 🔥 Callix – A Function-Only Programming Language  

FuncLang is a **C++-based interpreted programming language** where **everything is a function**.  
No keywords, no operators, no if-else, no loops — just **functions**.  

From variable declaration to arithmetic operations, control flow, and user-defined functions — **everything is expressed as function calls**.  

---

## ✨ Features  

✅ **Function-based Syntax** – Everything (declaration, printing, arithmetic, conditions, switch-case) is a function.  
✅ **Arithmetic as Functions** – `add(5, 3)`, `subtract(10, 4)`, `multiply(2, 3)`, `divide(8, 2)`, `modulus(10, 3)`  
✅ **Conditional Logic** – `check(condition, if_block, else_block)` works as if-else.  
✅ **Switch-like Control** – `switch(value, case1, block1, case2, block2, ..., default_block)`  
✅ **User-defined Functions** – Create new functions with `new("myFunc", params, body)` and reuse them.  
✅ **Dynamic Interpreter** – Custom parser + interpreter built in C++.  

---

## 📖 Example Code  

```text
declare("x", 10)
declare("y", 20)

print(add(x, y))         # → 30
print(multiply(5, 6))    # → 30

check(gt(x, y),
      print("x is greater"),
      print("y is greater or equal"))

switch(x,
       5, print("x is 5"),
       10, print("x is 10"),
       print("default case"))

new("greet", ["name"], print(add("Hello, ", name)))
greet("Nierdeh")
