**[What is Sampling?](https://en.wikipedia.org/wiki/Sampling_(signal_processing))
**

Imagine a continuous signal, such as sound, light, or voltage in an electrical circuit. This signal changes smoothly and continuously over time. Sampling is the process of converting such a continuous signal into a discrete signal.  That is, we take "snapshots" of the signal value at regular intervals.  These "snapshots" are called *samples*.

**Analogy:**

Imagine that you are photographing a moving object.  Each photo is a "countdown".  A series of photos shows the movement of an object in time, although the information between the photos is lost.  Sampling does the same thing, but for signals.

**Key concepts:**

1. **Continuous signal (Analog Signal):** A signal defined for each time point.  Example: sound, light, voltage in a circuit.
2. **Discrete Signal:** A signal defined only at discrete time points (samples).
3. **Sampling Rate (fs): ** The number of samples taken in one second. It is measured in hertz (Hz). The higher the sampling frequency, the more information we store about the original signal.
4. **Sampling Period (Ts):** The time between two adjacent counts.  This is the inverse of the sampling frequency: `Ts = 1 / fs'.
5. **Amplitude:** The magnitude of the signal at a certain point in time.
6. **Quantization:** The process of rounding the values of the sampling amplitude to the nearest discrete level (for example, in a digital system). This is a separate process that follows the discretization.

**The sampling process:**

1. **Sampling frequency selection (fs):** We decide how often we will take samples.
2. **Taking counts:** At regular intervals (sampling period `Ts = 1 / fs`), we measure the amplitude of the signal.
3. **Receiving a discrete signal:** We get a sequence of samples (numbers) representing the amplitude of the signal at different points in time.

**The Kotelnikov-Nyquist theorem (Nyquist–Shannon sampling theorem):**

This is a fundamental theorem that says how important it is to choose the right sampling frequency in order to be able to accurately reconstruct the original continuous signal from its discrete samples.

* **Formulation:** In order to perfectly restore an analog signal from its samples, the sampling frequency (`fs`) must be *at least twice* the maximum frequency (`f_max') in the original signal.
* **Formula:** `fs >= 2 * f_max`
* **Nyquist frequency (f_N):** Half of the sampling rate ('f_N = fs / 2`). The theorem says that we must sample the signal with a frequency at least twice as high as the maximum frequency contained in the signal. The Nyquist frequency is the maximum frequency that we can reliably represent at a given sampling rate.
* **Example:** If the source signal contains frequencies up to 10 kHz, the sampling frequency must be at least 20 kHz.

**What happens if the sampling rate is too low?  (Aliasing)**

If we sample a signal with a frequency that is lower than twice the maximum frequency (`fs < 2 * f_max`), then aliasing occurs. This means that the high frequencies in the original signal are "superimposed" on the lower frequencies in the discrete signal, and when we try to restore the original signal, we get distortion.

* **Analogy:** Imagine a wheel that turns fast.  If you shoot it with a camera with a low frame rate, you may see that the wheel rotates slower, or even in the opposite direction.  This is an example of aliasing.

**Examples:**

* **Sound:**
    * The human ear hears frequencies from about 20 Hz to 20 kHz.
    * For audio sampling, a frequency of 44.1 kHz (CD-quality) or 48 kHz (DVD-quality) is usually used. This ensures that all audible frequencies are correctly represented (with a margin to avoid aliasing).
* **Video:**
* The frame rate (frames per second, FPS) in a video is, in fact, the sampling rate for a signal representing motion.
    * Standard frame rates: 24 FPS (movies), 30 FPS (TV), 60 FPS (video games).
* **Electrical signals:**
    * Digital oscilloscopes use sampling to measure voltage over time.  The sampling rate must be high enough to accurately display voltage changes.

**The effect of sampling on the signal (in the frequency domain):**

Sampling affects the spectrum of the signal.  When sampling an ideal signal, the following happens:

1. **Repetition of the spectrum:** The spectrum of the original signal is repeated with an interval equal to the sampling frequency (`fs`).
2. **Aliasing (if fs < 2*f_max):** If the sampling frequency is insufficient, the repeating spectra overlap each other.  This leads to distortion (aliasing).
3. **Amplitude-frequency response (frequency response) of an ideal reconstruction filter:** When the signal is perfectly restored (interpolated), an ideal low-pass filter is applied, which cuts off all frequencies above the Nyquist frequency.  This allows you to restore the original signal without distortion (if the Nyquist theorem was observed).

**Filtering:**

To prevent aliasing, a low-pass filter is usually used before sampling.  This filter removes all frequencies that exceed half of the sampling frequency (Nyquist frequency).  This ensures that the original signal does not contain frequencies that can cause aliasing.

**Sampling methods:**

There are various sampling methods:

* **Uniform sampling (Uniform Sampling):** The counts are taken at regular intervals.  This is the most common method.
* **Non-uniform sampling (Non-uniform Sampling):** The readings are taken at irregular intervals.  It is used in certain applications.

**Practical application:**

Sampling is a fundamental process in modern digital signal processing. It is used in:

* **Audio (digital audio):** Record, store and play audio (CD, MP3, WAV, etc.).
* **Video (digital video):** Video recording, storage and playback (movies, TV, YouTube, etc.).
* **Telecommunications:** Transmission of voice and other signals over telephone lines, radio and the Internet.
* **Medical equipment:** ECG, EEG, MRI and other medical devices that use digital signals.
* **Measurement and control:** Collecting data from sensors for various applications (e.g. meteorology, industry).
* **Image Processing:** Convert images to digital format.

**A short summary for the post:**

 Sampling is a key process that converts continuous signals into digital ones.
* It is important to choose the right sampling frequency (Nyquist frequency) to avoid distortion (aliasing).
* Sampling is at the heart of digital signal processing and is used in a variety of modern technologies.