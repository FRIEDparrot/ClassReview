### (1) Basic Concepts  
The core concept is :  
1. Using Bounding Box to describe the location  
2. Convert  into  `center, width, height` reprensentation   
3. Cutting out the target 


Prediction : 
1. **Generate multiple Anchor Boxes** 

We note for the image with width $w$ and height $h$,  **if we set a series of scales** $s_{1}, s_{2}, \dots  s_{n}$,   and a series of aspect ratio  $r_1, \dots r_{m}$, 
The complexity will be : 
$$w\times  h \times n \times  m $$

so in practive,  <b><mark style="background: transparent; color: orange">we can only consider the  combination that contains the </mark></b> $s_{1}$ or $r_{1}$, that lead to the choice of  following: 
$$(s_{1}, r_{i}) ,  (s_{j},r_{1}) \rightarrow   m + n - 1 \space \text{choice}$$
then the total  <mark style="background: transparent; color: red">generated  bboxs number</mark>  reduce to $wh(n + m - 1)$, creating the following bounding boxes (3 + 3 - 1): 
![[attachments/Pasted image 20250617173548.png|300]]
We classify each anchored box by `background` (often as class 0). 

We represent a true-ground box with 5 parameters : 
$$(class,  topx,  topy, btmx, btmy)$$
note also <b><mark style="background: transparent; color: orange">achors are also reprensented by</mark></b>
$$(topx,  topy, btmx, btmy)$$
where  $s$ is  **scale**, $r$ is **aspect ratio** (ratio of width to height) 

> [!HINT] Offset Prediction Method 
> For each Image, we <b><mark style="background: transparent; color: orange">generate multiple  anchor boxes, predict classes and offsets for all the anchor boxes, and adjust  their positions according to the  predicted offsets</mark></b> 
> 
> An object detection **training set comes with labels for locations of ground-truth bounding boxes and classes of their surrounded objects** 
> We always  assign closest  ground-truth  bounding boxes  to anchor boxes. 

if an anchor box is not assigned a  ground-truth bounding box, we will classify it as "background" 
We note that  given the central coordinates  of $A(x_a, y_a)$ and $B(x_{b}, y_{b})$,  and their width are $w_a$ and $w_b$,  then **the label of offset** is calculated as : 
$$\left(\frac{\frac{x_{b}- x_{a}}{w_{a}} - \mu_{x}}{\sigma_{x}} , \frac{\frac{y_{b} - y_{a}}{h_{a}} - \mu_{y}}{\sigma_{y}},  \frac{\log_{}\frac{w_{b}}{w_{a}} - \mu_{w} }{\sigma_{w}},   \frac{\log_{}  \frac{h_{b} }{h_{a}} -  \mu_{h}}{\sigma_{h}} \right)$$

> [!NOTE] Default  Value 
> We often use  $\mu_x = \mu_y  =\mu_w = \mu_h  = 0, \sigma_x = \sigma_y  = 0.1,\sigma_{w} = \sigma_{h} = 0.2$

### (4) Result Output   
To simplify the output, we can <b><mark style="background: transparent; color: orange">merge similar predicted bounding boxes that belong to the same object</mark></b> by using non-maximum suppression (NMS). 

All the predicted  non-background bounding boxes are sorted by confidence  in descending oorder to generate a list  $L$

1. Select the predicted bounding box B1 with the highest confidence from L as a basis and <mark style="background: transparent; color: red">remove all non-basis predicted bounding boxes whose IoU with B1 exceeds a predefined  threshold ϵ from L</mark>. At this point, L keeps the predicted bounding box with the highest confidence but **drops others that are too similar to it**. In a nutshell, those with non-maximum confidence scores are suppressed. 
2. Select the predicted bounding box B2 with the second highest confidence from L as another basis and<mark style="background: transparent; color: red"> remove all non-basis predicted bounding boxes whose IoU with B2 exceeds ϵ from L</mark>.

In this process, the bounding box  that with lower confidence are removed.  

> [!warning] 
> However, the traditional  NMS can only use confidence and $\varepsilon$  for sorting, which can lead to incorrect bounding. 

Also, in  IoU aware Confidence, we have this in yolov4 : 
$$confidence  = Pr  \times    IoU$$

## 3. Image detection Example by Yolov8 

```python fold title:setup 
import ultralytics 
ultralytics.checks()
```
