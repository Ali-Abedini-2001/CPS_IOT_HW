# CPS_IOT_HW
RFID door lock controlling Using ESP8266 module 

## Report
**اعضای گروه:**
- <p>حامد میر امیرخانی 810199</P>
  
- <p>محمد امین پورزارع 810199</p>
- <p>پارسا ناصری 810199</p>
- <p>علی عابدینی 810199555</p> 
## Server Side

## Embedded Side

## پاسخ سوالات

**_سوال 1_**

برای استفاده از بلوتوث به جای RFID برای کنترل موتور DC و باز کردن یک ماژول اترنت Arduino و ENC28J60، به یک ماژول بلوتوث مانند HC-05 یا HC-06 نیاز دارید. شما باید این ماژول را به آردوینو متصل کنید. ماژول بلوتوث را می توان با یک دستگاه دارای بلوتوث مانند گوشی هوشمند جفت کرد. هنگامی که دستوری از گوشی هوشمند ارسال می شود، ماژول بلوتوث آن را دریافت کرده و به آردوینو ارسال می کند. سپس آردوینو دستور را پردازش می کند و اقدامات لازم مانند کنترل موتور DC یا باز کردن ماژول اترنت را انجام می دهد.

---

**_سوال 2_**

ماژول های بلوتوث معمولاً قبل از تبادل اطلاعات نیاز به جفت شدن دارند. این یک ویژگی امنیتی است که برای جلوگیری از اتصال و تبادل داده های دستگاه های غیرمجاز طراحی شده است. با این حال، برخی از ماژول‌های بلوتوث وجود دارند که از انتقال داده‌ها بدون جفت شدن پشتیبانی می‌کنند، اما این ماژول‌ها معمول نیستند و ممکن است ایمن نباشند. همچنین شایان ذکر است که حتی اگر انتقال داده بدون جفت شدن امکان پذیر باشد، به دلیل خطرات امنیتی احتمالی توصیه نمی شود.

---

**_سوال 3_**

به جای استفاده از تایمر برای بستن ورودی شرکت، می توانید از سیستم قفل دارای بلوتوث استفاده کنید. کارمندان می توانند یک دستگاه دارای بلوتوث، مانند گوشی هوشمند، داشته باشند که سیستم قفل آن را تشخیص دهد. هنگامی که سیستم دستگاه را در محدوده خاصی تشخیص می دهد، می تواند به طور خودکار قفل ورودی را باز کند. هنگامی که دستگاه خارج از محدوده حرکت می کند، سیستم می تواند به طور خودکار ورودی را قفل کند. این راه حل نیاز به تایمر را از بین می برد و به طور بالقوه می تواند ایمن تر باشد، زیرا قفل ورودی فقط برای دستگاه های شناخته شده باز می شود. با این حال، کارمندان نیز باید همیشه دستگاه دارای بلوتوث خود را به همراه داشته باشند و بلوتوث دستگاه نیز روشن باشد.

---

<style>
    p{
        text-align: right;
    }
</style>