## 🔥 INLINE vs. NORMAL FUNCTIONS: What's Really Happening in Assembly?

Everyone talks about 'inline', but few have seen the difference in real assembly. See:

### 🎯 With INLINE — code is INSERTED directly:
```asm
test_with_inline:
movl (%rdi), %r8d # Byte comparison
cmpl (%rsi), %r8d # ← code INSERTED!
je .L18

movl (%rsi), %ecx
cmpl (%rdx), %ecx # ← INSERTED again!
je .L19
```
**The gist:** The compiler COPYS the function code to each call site.

### 🐌 WITHOUT INLINE — CALLS to the function:
```asm
test_without_inline:
call mac_equals_normal # ← JUMP into the function!
movzbl %al, %ecx

call mac_equals_normal # ← JUMP AGAIN!
movl %eax, %esi
```
**The gist:** Each call is a jump to a different memory location

### 🍔 SIMPLE ANALOGY:
- **Without inline** = "Courier: you → courier → restaurant → courier → you"
- **With inline** = "Microwave: you → microwave → done!"

### ⚡ RESULT:
- **With inline:** ~4 instructions per comparison
- **Without inline:** ~20+ instructions (call + return)

### 🎯 CONCLUSION:
`inline` is when the compiler stops "sending the courier" and starts "microwave" right there!

**Command for testing:**
```bash
gcc -S -O2 test.c -o test.s
```
