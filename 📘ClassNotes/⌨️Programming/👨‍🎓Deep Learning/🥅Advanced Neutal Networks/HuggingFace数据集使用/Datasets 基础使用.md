##  一、基础查看和使用方法  
### (1) 依赖安装
```sh
pip install datasets torch transformers 
pip install datasets[audio] 
pip install datasets[vision]
pip install huggingface_hub[hf_xet]   # download 
pip install accelerate  # 解决 # name 'init_empty_weights' is not defined 
```

注意其中的所有缓存数据集均可在 : 
```
C:\Users\Parrot\.cache\huggingface\datasets 
```
(前面为用户目录) 中找到，

### (2) 
```python
print(dataset.info)  
print(dataset.column_names)  # 查看所有的字段 

for value in dataset["audio"]:
    print(value)  # 输出所有数据的 audio 字段 
# 查看某个字段对应的前  10 样本 
dataset.select(range(10))['audio'] 
# 随机获取5个样本
samples = dataset.take(5)

dataset.to_pandas()  # `to_pandas` 转化数据集为  DataFrame
dataset.to_pandas().head(10)   #  查看前 10 个 
```

也可以使用 :  
```python
from rich import print 
print(dataset[0]) 
```


对于 wav2vec2 数据集， 可以整体查看
```python
print(dataset[0]["audio"]) 
{'path': '602ba55abb1e6d0fbce92065.wav', 'array': array([ 1.70561689e-05,  2.18727393e-04,  2.28099860e-04, ...,
        3.43842403e-05, -5.96366226e-06, -1.76846370e-05]), 'sampling_rate': 16000}
```

标签和 id 对应获取 :  
```python 
labels = dataset.features["intent_class"].names  # get the list of intent labels  
id2label = {i: label for i, label in enumerate(labels)}  
print("intent labels: \n", labels) 

intent labels: 
 ['abroad', 'address', 'app_error', 'atm_limit', 'balance', 'business_loan', 'card_issues', 'cash_deposit', 'direct_debit', 'freeze', 'high_value_payment', 'joint_account', 'latest_transactions', 'pay_bill']  
```


在模型进行训练时,   需要在 https://wandb.ai/  上注册账号， 并采用 : 
```python
pip install wandb  
wandb login  
```

直接使用如下代码进行测试, 对于个人用户， 无需指定 entity 部分; 

```python
import random  
import wandb  
  
# Start a new wandb run to track this script.  
run = wandb.init(  
    #  for personal  
    # Set the wandb project where this run will be logged.    project="test-project",  
    # Track hyperparameters and run metadata.  
    config={  
        "learning_rate": 0.002,  
        "architecture": "CNN",  
        "dataset": "CIFAR-100",  
        "epochs": 50,  
    },  
)  
  
# Simulate training.  
epochs = 10  
offset = random.random() / 5  
for epoch in range(2, epochs):  
    acc = 1 - 2 ** -epoch - random.random() / epoch - offset  
    loss = 2 ** -epoch + random.random() / epoch + offset  
  
    # Log metrics to wandb.  
    run.log({"acc": acc, "loss": loss})  
  
# Finish the run and upload any remaining data.  
run.finish()
```

实际上后面的部分， 即相当于模拟训练过程，  显然在 epochs 中迅速训练迭代完成。 计算公式为: 
$$acc =  1- \frac{2^{-epoch}}{epoch  - offset}$$
部分，  只需 `run.log` 即可进行可视化训练。 

![[attachments/Pasted image 20250408171649.png]] 

对于常见使用的 Hogh-level API, 例如 PipeLine,  

```python
from transformers import pipeline  

classifier  = pipeline("sentiment-analysis")
classifier("test1") 
```

对于一般的 PipeLine, 常用办法有如下 (对于全部的 `task` 参数， 可参考  https://huggingface.co/docs/transformers/main/en/main_classes/pipelines ) 

| Task                         | Description                                                                                                  | Modality        | Pipeline identifier                           |
|------------------------------|--------------------------------------------------------------------------------------------------------------|-----------------|-----------------------------------------------|
| Text classification          | assign a label to a given sequence of text                                                                   | NLP             | pipeline(task=“sentiment-analysis”)           |
| Text generation              | generate text given a prompt                                                                                 | NLP             | pipeline(task=“text-generation”)              |
| Summarization                | generate a summary of a sequence of text or document                                                         | NLP             | pipeline(task=“summarization”)                |
| Image classification         | assign a label to an image                                                                                   | Computer vision | pipeline(task=“image-classification”)         |
| Image segmentation           | assign a label to each individual pixel of an image (supports semantic, panoptic, and instance segmentation) | Computer vision | pipeline(task=“image-segmentation”)           |
| Object detection             | predict the bounding boxes and classes of objects in an image                                                | Computer vision | pipeline(task=“object-detection”)             |
| Audio classification         | assign a label to some audio data                                                                            | Audio           | pipeline(task=“audio-classification”)         |
| Automatic speech recognition | transcribe speech into text                                                                                  | Audio           | pipeline(task=“automatic-speech-recognition”) |
| Visual question answering    | answer a question about the image, given an image and a question                                             | Multimodal      | pipeline(task=“vqa”)                          |
| Document question answering  | answer a question about a document, given an image and a question                                            | Multimodal      | pipeline(task="document-question-answering")  |
| Image captioning             | generate a caption for a given image                                                                         | Multimodal      | pipeline(task="image-to-text")                |


```python
pipeline = pipeline(task="automatic-speech-recognition", model="openai/whisper-large-v3") 
pipeline(audio="https://huggingface.co/datasets/Narsil/asr_dummy/resolve/main/mlk.flac", return_timestamp="word")
```

一般而言目前常见的模型都可以直接找到， 并且采用 pipeline 进行调用。 
```python
# Use a pipeline as a high-level helper
from transformers import pipeline

messages = [
    {"role": "user", "content": "Who are you?"},
]
pipe = pipeline("text-generation", model="deepseek-ai/DeepSeek-V3-0324", trust_remote_code=True)
pipe(messages) 
```

对于  Hugging Face 中, 我们将 model 进行训练完成之后, 一般是使用  pipeline 进行加载的方式， 
```python
from transformers import pipeline

classifier = pipeline("audio-classification", model="stevhliu/my_awesome_minds_model")
classifier(audio_file)
```
其中上述是 Hugging Face 上的公开模型，  一般地， 训练模型之后， 可以直接发布模型到 Hugging Face. 
`model`参数支持以下三种来源：
1. **Hugging Face Hub上的公开模型**（如示例中的`stevhliu/my_awesome_minds_model`）：
   ```python
   classifier = pipeline("audio-classification", model="stevhliu/my_awesome_minds_model")
   ```
2. **本地保存的模型路径**（需包含完整的模型文件和配置文件）：
```python
pt_save_directory = "./pt_save_pretrained"   
tokenizer.save_pretrained(pt_save_directory)
pt_model.save_pretrained(pt_save_directory) 
# 加载本地模型部分
classifier = pipeline("audio-classification", model="./pt_save_pretrained")   
```

3. **通过Git分支或提交哈希引用Hub上的模型**（适用于需要特定版本的情况）：
   ```python
   classifier = pipeline("audio-classification", model="stevhliu/my_awesome_minds_model@main")
   ```

需要说明，模型必须通过`model.save_pretrained("./my_local_model/")`保存，确保包含以下文件：
  - `config.json`（模型配置）
  - 模型权重文件（如`pytorch_model.bin`、`tf_model.h5`等）
  - 必要的预处理配置文件（如`preprocessor_config.json`）

最常见的加载预训练模型的部分包含 : 
```python
from transformers import * 

AutoModelForAudioClassification.from_pretrained("model_name"),  
AutoFeatureExtractor.from_pretrained()  
model = AutoModelForSequenceClassification.from_pretrained(model_name)
tokenizer = AutoTokenizer.from_pretrained(model_name) 
```

在Hugging Face Transformers库中，`facebook/wav2vec2-base` ​**是一个统一的预训练模型基座**，但通过不同的`AutoModel`子类加载时，会根据任务需求动态添加不同的头部（Task Head）。

- ​**基座模型**：`facebook/wav2vec2-base` 是一个**通用语音表示学习模型**​（如语音特征提取），其核心架构是固定的，例如包含**卷积特征编码器** + Transformer层。
- ​**任务头部**：通过不同的`AutoModelForXXX`类，可以在基座模型上添加不同的任务头部：
    - `AutoModelForAudioClassification`：添加分类头部（全连接层）。
    - `AutoModelForCTC`：添加CTC头部（语音识别任务）。
    - `AutoModelForPreTraining`：保留预训练任务的头部（如对比学习）。  

一般地， 每个模型页面的 

## 二、上传模型到 HuggineFace 账号 

```python
model.push_to_hub("your-username/private-model", private=True)
trainer.push_to_hub("friedparrot/test-model1")   # 可用于测试 
``` 

如果仅仅限于使用对应的模型,  一般 Auto 方法中给出了大量的模型定义 :   
```python
from transformers import AutoTokenizer
tokenizer = AutoTokenizer.from_pretrained("google/gemma-2-2b")
```

上述方法和以下加载方法一致 :  
```python
from transformers import GemmaTokenizer
tokenizer = GemmaTokenizer.from_pretrained("google/gemma-2-2b")
```

