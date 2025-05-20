
## [RFC 1123](https://www.rfc-editor.org/rfc/rfc1123.html#page-7): Robustness Principle & Error Logging - A Summary

This document outlines critical principles for building robust and reliable internet software, focusing on handling errors and potential misbehavior. Here's a structured breakdown:

**1. The Robustness Principle: "Be liberal in what you accept, and conservative in what you send."**

This core principle is divided into two vital parts:

* **Liberal Acceptance:**  Software *must* be designed to gracefully handle *any* conceivable error, no matter how improbable. The internet will, eventually, encounter it.  Assume a hostile environment – packets may be crafted with malicious intent.  Don't rely on perfect input; prepare for the worst.  Unexpected events, even those not caused by malicious actors, are often the root cause of major internet problems.
* **Conservative Sending:** Avoid exploiting obscure or ambiguous protocol features. Stick to established, simple methods.  Other systems might not handle complex or non-standard behavior correctly, leading to unintended consequences. Be mindful of the potential for misbehaving hosts and design systems to tolerate and even *limit* disruption caused by them.

**Key takeaway:**  Design for resilience.  Expect errors, and don't contribute to the problem by being overly clever or relying on unverified features.  Adaptability is crucial.

---

**2. Error Logging: Diagnosing and Managing Issues**

Effective error logging is essential for identifying and resolving problems. The RFC recommends:

* **Comprehensive Logging:** Implement a system for logging erroneous or unusual protocol events.
* **Manage Log Overflow:** Prevent logs from becoming overwhelmed with harmless anomalies by employing:
    * **Circular Logs:**  Overwrite old entries.
    * **Selective Logging:** Enable logging only during active troubleshooting or for specific scenarios.
    * **Filtering & Counting:**  Suppress redundant, successive identical messages.
* **Strategic Implementation:**
    * **Abnormality Counters:**  Track counts of unusual events and make these accessible via a management protocol.
    * **Granular Control:** Allow selective enabling of logging for various event types (e.g., "log everything," "log events for host X").

**Important Consideration:**  Error logging policies will vary between management domains. Some may prioritize minimal logging ("if it doesn't affect me, ignore it"), while others will favor vigilant monitoring of protocol anomalies.




