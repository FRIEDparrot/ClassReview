#### This is Graphic Practice!

```mermaid
graph LR 
A[Problem Analysis]--> B
B[Text] --> C{Decision}
C -->|One| D[Result 1] 
C -->|Two| E[Result 2]
```
```mermaid 
graph LR 
A["Something"]:::someclass --> B 
B --> C 
A --> C
D --> C 
classDef someclass fill:#f96;
```


```mermaid
graph TB
A
A[Question] --> |analysis|B
B --> |predict|C[result analysis]
C --> D[low precision]
D --> |optimization|E[parameter Choose]
D --> |error analysis|F[accuracy evaluation]
F --> H[RMSE/MSE error analysis]
E --> |retrain model|B

C --> G[high precision]
C --> H
G --> I[over fitting analysis]
I --> |train-test-split| J

```

















