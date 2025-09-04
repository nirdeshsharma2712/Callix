declare("x", 10);
declare("y", 5);

print("sum:", add(get("x"), get("y")));     // 15
print("prod:", multiply(2,3,4));            // 24
print("sub:", subtract(10,3,2));            // 5
print("div:", division(20, 2, 5));          // 2
print("mod:", mod(10, 3));                  // 1

// if/else-if/else via check
check(
  gt(get("x"), 20),   lambda0(print("x>20")),
  gt(get("x"), 8),    lambda0(print("x>8 and <=20")),
  lambda0(print("else branch"))
);

// switch
switch(
  get("y"),
  1, lambda0(print("y is 1")),
  5, lambda0(print("y is 5")),
  lambda0(print("y is something else"))
);

// change variable
set("x", add(get("x"), 30));
check(
  gt(get("x"), 20), lambda0(print("now x>20"))
);

// define a zero-arg function and call it by name
newname("hello", lambda0(print("Hello from user function!")));
hello();

// compose
print("done");
