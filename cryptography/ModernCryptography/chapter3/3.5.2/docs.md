# 3.5.2 CPA-Security from a Pseudorandom Function

## THEOREM 3.29 
If **F** is a pseudorandom function, then Construction 3.28
is a CPA-secure, fixed-length private-key encryption scheme for messages of
length **n**.

## PROOF: 
\left| \Pr\left[ \PrivK_{\mathcal{A},\Pi}^{\text{CPa}}(n) = 1 \right] - \Pr\left[ \PrivK_{\mathcal{A},\Pi}^{\text{CPa}}(n) = 1 \right] \right| \leq \mathsf{negl}(n). \quad (3.9)

<!--
<script src="https://polyfill.io/v3/polyfill.min.js?features=es6"></script>
<script id="MathJax-script" async src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-mml-chtml.js"></script>
-->

