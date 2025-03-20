To accelerate image convolution using the convolution theorem, follow these steps:

### **Key Concept: Convolution Theorem**
The theorem states that convolution in the **spatial domain** is equivalent to **element-wise multiplication** in the **frequency domain**. This allows replacing computationally intensive spatial convolution with faster Fast Fourier Transform (FFT) operations.

### **Steps to Accelerate Image Convolution**

1. **Padding**:
   - **Goal**: Avoid circular convolution artifacts (wrap-around errors) and ensure linear convolution.
   - **Method**: Pad both the image and kernel to size `(N + K - 1) × (N + K - 1)` where:
     - `N` = width/height of the original image.
     - `K` = width/height of the kernel.
   - **Example**: For a `256×256` image and `5×5` kernel, pad to `260×260`.

2. **Fourier Transform**:
   - Compute FFT of the padded image (`FFT(image)`) and padded kernel (`FFT(kernel)`).
   - **Note**: For true convolution (not cross-correlation), flip the kernel spatially before padding. In many applications (e.g., CNNs), cross-correlation is used, so flipping is skipped.

3. **Frequency Domain Multiplication**:
   - Multiply the FFT results element-wise:  
     `FFT_result = FFT(image) ⊙ FFT(kernel)`.

4. **Inverse Fourier Transform**:
   - Compute the inverse FFT of `FFT_result` to return to the spatial domain:  
     `result = IFFT(FFT_result)`.

5. **Cropping**:
   - Crop the output to the original image size (`N×N`) for "same" convolution, or to `(N - K + 1)×(N - K + 1)` for "valid" convolution.

---

### **Why This is Faster**
- **Direct Convolution Complexity**:  
  \( O(N^2 K^2) \) for an \( N×N \) image and \( K×K \) kernel.
- **FFT-Based Convolution Complexity**:  
$$ O(N^2 \log N) $$
(dominated by the FFT steps).
- **Efficiency**: FFT-based methods outperform direct convolution for large kernels (typically \( K > 15 \)).

---

### **Practical Considerations**
- **Kernel Symmetry**: Symmetric kernels (e.g., Gaussian blur) do not require flipping.
- **Edge Handling**: Use zero-padding or replicate edges to minimize artifacts.
- **Memory Overhead**: Padding increases memory usage, but optimized FFT libraries (e.g., FFTW, cuFFT) mitigate this.
- **Precomputation**: Reuse precomputed FFT(kernel) if applied to multiple images.

---

### **Example Workflow**
```python
import numpy as np
from scipy.fft import fft2, ifft2, fftshift

def fft_convolve(image, kernel):
    # Pad image and kernel
    image_padded = np.pad(image, ((0, kernel.shape[0]-1), (0, kernel.shape[1]-1)))
    kernel_padded = np.pad(kernel, ((0, image.shape[0]-1), (0, image.shape[1]-1)))
    # Compute FFTs
    fft_image = fft2(image_padded)
    fft_kernel = fft2(kernel_padded)

    # Multiply in frequency domain
    result_freq = fft_image * fft_kernel

    # Inverse FFT and crop
    result = np.real(ifft2(result_freq))
    result = result[:image.shape[0], :image.shape[1]]  # "same" convolution
    return result
```

---

### **When to Use FFT-Based Convolution**
- Large kernels (\( K > 15 \)).
- Batched processing (reuse FFT(image) for multiple kernels).
- High-resolution images where direct convolution is impractical.

By leveraging the convolution theorem, you achieve significant speedups for large-scale image processing tasks. 