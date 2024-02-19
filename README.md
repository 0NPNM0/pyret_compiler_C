a simple complier for pyret language.
scanner 100%
parser 80%
sementic analyzer 0%

scanner details：

1.Define tokens：
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/23437b3c-dc7f-4eb3-bfe5-29c261367f06)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/efb2c0cb-b28b-4cf3-9e49-9b1b191db3a3)


2.Define Structure of Token, TokenNode and TokenList:
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/ad3f458c-8a7a-4fa8-9d59-1e814baa8a5c)


3.Define DFA States：
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/8059d3b1-296a-4890-bcae-998e3ef32f4b)

4.Define methods:
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/ae7fcafd-ff2a-4581-8c3d-ec640f38ec21)

5.Tests:
(1)text programme
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/435b8b1c-c219-4f9d-8a7e-b6415b1b8cef)


result:

![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/ac5b1446-5dd3-4f7f-9e07-434f6e5b802a)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/ffe76e59-ed6a-497e-9d2b-9e65d34d0d0d)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/0d4cdfbe-f67f-4422-8abb-1a3682c95cac)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/a5aa4338-4323-4b93-96e6-386083181c55)


(2)text programme
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/c3b5a4b7-e3a3-4bc3-98ec-e07f186096ed)


result:

![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/e67506e6-bf97-4ea5-a7ce-b0944e659466)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/ea70ee25-04df-4e7b-8c5a-c38932e6fdd4)


(3)text programme
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/e853b848-2dd3-4401-aa35-2fe06d53cb8c)


result:

![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/530a4269-29e8-4628-b674-ae1df78d8ec4)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/0217709b-62de-4793-a1ab-766c6aadb2aa)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/9dc142b6-3a01-4a22-b088-ee858ee4fff3)


(4)text programme
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/e80d57bb-f34e-4d11-9784-a094ee6e5a5a)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/3f1c5369-17aa-4990-886a-d22c89079f72)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/6b0dddff-8b68-4ddc-bc4d-74b3aa52d4f9)


result:

![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/06682e9d-2597-4625-8783-c3f0256da095)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/98f7d738-af15-4912-9438-05beba48ab1e)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/ce080788-15f8-4a84-b519-3034923d49d9)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/a7ae845f-ecef-49f6-91ec-e974af3fd980)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/bc8b4cd7-e42a-4192-b48d-5cd679a04409)


parser details：

[parser report.docx](https://github.com/0NPNM0/pyret_compiler_C/files/14331832/parser.report.docx)
