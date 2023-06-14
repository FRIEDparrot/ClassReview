首先需要对于源场进行处理
因此将所有的point以及edge 映射到原先的场上

注意Point和edge都需要进行映射(这主要是由于网格是基于点进行生成的)

![[attachments/Pasted image 20230607161703.png]]

需要将翼型的直线先映射好，然后将后方的两个点进行分别映射到翼面上
![[Excalidraw/wing 2023-06-07 16.49.02]]
在进行完毕截面的映射之后，进行对后缘的机翼的两条线的映射过程

白色`->` 面映射 

![[attachments/Pasted image 20230607164528.png]]

需要勾选around block 使得在沿屋面时候， 

![[attachments/Pasted image 20230607164838.png|650]]

需要注意的是, 选择面需要选机翼的下缘面， 
按照如图的方法进行网格的重新调整: 
![[attachments/Pasted image 20230607170111.png|300]]
在调整网格之后，得到的效果如下图： 
![[attachments/Pasted image 20230607170317.png|700]]

copy absolute `->` 常常在附面层中进行使用 , 保证附面层中的网格密度
![[attachments/Pasted image 20230607170851.png|500]]

对于三维网格， 往往需要在pre-mesh中代入scan plane `->` **截取特定平面的网格**来进行观察 
![[attachments/Pasted image 20230607171811.png|340]]

需要调整过渡和