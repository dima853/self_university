
# [RFC 1123](https://www.rfc-editor.org/rfc/rfc1123.html#page-7): Robustness Principle & Error Logging - A Summary

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

# RFC 1123: Configuration - A Summary

This section of RFC 1123 addresses the challenges and necessities of configuration within internet protocol implementations. While a fully self-configuring internet suite is the ideal, it remains a distant goal. Here's a breakdown of the key points:

**1. The Ideal vs. Reality:**

* **Desired State:** Complete self-configuration would simplify deployment (ROM/silicon implementation, diskless workstations), and significantly ease the burden on network administrators and vendors.
* **Current State:** We are far from achieving this ideal.

**2. Reasons for Configurable Parameters:**

The RFC identifies several reasons why parameters *must* be configurable:

* **Uncertainty/Disagreement:**  Optimal values for some parameters are currently unknown or debated, and may need adjusting in the future.
* **External Dependencies:**  Values are influenced by external factors like host size, network load, and network topology.  Self-tuning algorithms are not always sufficient.
* **Administrative Needs:**  Specific administrative requirements dictate certain parameter settings.
* **Backward Compatibility:**  To interact with older, flawed, and often source-code-unavailable implementations that still exist on the internet, systems sometimes need to be "mis-configured."

**3. Implementation Guidance for Configurability:**

* **Defaults are Key:** Implementors should establish sensible default values for all configurable parameters.  Configuration files should only be *required* to override these defaults when necessary.
* **Accessibility of Configuration:**  The requirement for configurability ensures the *possibility* of overriding defaults, even in binary or ROM-based products.

**4. The Importance of Standard Defaults:**

* **Convergence to Interoperability:**  To ensure a successful transition to complete interoperability across the internet, default values should adhere to the *official protocol specification*, not workaround "mis-configurations" for faulty systems.
* **Vendor Responsibility:**  The RFC strongly urges vendors to prioritize adherence to the standard in their default configurations, despite potential marketing pressure to accommodate older, problematic systems.

In essence, this section acknowledges the pragmatic need for configuration in the face of incomplete standards and existing legacy systems. It emphasizes choosing standard-compliant defaults to facilitate future interoperability, while also providing flexibility to accommodate specific environments and flawed implementations.



