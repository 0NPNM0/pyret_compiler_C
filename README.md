a simple complier for pyret language.
scanner 100%
parser 80%
sementic analyzer 0%

<1>  scanner details：

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


<2>  parser details：
1. Implemented grammars:

《program》
<program> => (<stmt>)+

《expression》
<expr> =>  <non-op-expr>  | <binop-expr> | <unop-expr>
<non-op-expr> => <id> | <literal> | <fun-call> | <paren-expr> | <if-expr> | <block> | <construct-expr> | <lambda-expr>
<unop-expr> => <unop> ( <non-op-expr> |  <paren-expr> )  
<unop> => not
<binop-expr> => <non-op-expr> <binop> ( <non-op-expr> | <paren-expr> )
<binop> => [+ , - , * , / , %, < , <= , > , >= ,  ==]
<paren-expr> => \( <expr> \)
<if-expr> =>
   if <expr> : <block> (<else-if>)+  (else: <block>)? end |
   if <expr> : <block> (<else-if>)*  else: <block> end
‹else-if› =>  else if <expr> : <block>
<fun-call> => <id>  \(  (  | (<expr> ,)* <expr> )  \)
<block> => <stmt> | block :  <stmt>* end
<construct-expr> => <array> | <list>
<list> => \[ list: (  | (<expr> ,)* <expr> ) \]
<array> => \[ array: (  | (<expr> ,)* <expr> ) \]
<lambda-expr>  => lam <fun-header> : <block> end

《statement》
<stmt> =>  <when-stmt> | <assign-stmt> | <expr> |
            <decl-stmt> | <binding-stmt>
<stmts> => (<stmt>) *
<assign-stmt> => <id> := <expr> 
<when-stmt> => when <expr> : <block> end
<decl-stmt> => <var-decl> | <fun-decl>
<var-decl> => (shadow)? (rec)? var <id> = <expr>
<fun-decl> => (shadow)? (rec)? fun <id> <fun-header> : <stmts> end
<fun-header> => \( < params>  \) (<return-part>)
<return-part> =>  -> <type-name>
<params> =>   | <param_list>
<param_list> => (<param> ,)* <param>
<param> =>  (shadow)? <id> [:: <type-name>]
<binding-stmt> => <id> = <expr>
 
《literal》
<literal> => <boolean> | <string> | <nothing> | <number> 
<boolean> =>  true | false
<string> => <single_quote_str> | <double_quote_str> | <multi_line_str>
<single_quote_str> => ' ([ASCII]-[' \n] | \')* '
<double_quote_str> => " ([ASCII]-[" \n] | \")* "
<nothing> => 
<number>  =>  <integer> 
<integer> => [- \+]? [0-9]+

《names》
<id> => (<name>) – (<keyword>) 
<type-name> => Any | Number | String | Boolean | Nothing | Function

2.Test results with simple correct Pyr code:
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/f37ae4a9-6f79-44b5-8f01-0e2c57184d59)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/7ec75729-bb4a-4217-be32-a32e479b17fe)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/ece0de0d-6013-41c8-a5ea-bd115958253f)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/e13aca27-06ad-43af-927e-04532c3e02b9)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/c7e7fce9-6d08-42b4-95eb-3038953be3a1)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/c1351e20-7f0a-48cf-ac61-7d050f6bf209)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/268e5d6c-aa6e-4677-b817-5b5a9612b051)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/6eb2d4ef-1e6c-4b53-87f6-2a9c821c4bb5)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/47b462ff-c9df-499f-b1af-36ac34daeb0a)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/6d42b65d-810b-49f1-9ec6-e69ab64e5c55)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/104da55e-fe35-4326-ab46-d2159d03f9b7)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/d93576f2-69a5-4957-956c-0d3adccb9fa4)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/f1bb228b-17b6-4ed0-9d09-e3da3f482521)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/9f2d383c-52f2-4214-bae3-4798fa294ed0)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/de6fbfc6-ccc4-4cf7-b548-86bccf7069bb)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/849d9e31-c2bf-40ba-8321-9604caac9450)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/968d790e-cc04-46fd-afb3-e83b4dbeef98)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/c56926a9-b54c-4caa-8d74-9b41819afb17)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/5de2bf39-250e-44a7-b87f-c4e2876de65a)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/be09dab1-213d-4250-848a-ba57321021fb)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/522e2ffa-2360-4d22-a855-8491f2c68933)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/a882bc57-b2b7-4c08-8e6e-4011d52525f3)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/3c15a42f-a5c1-41bf-ade0-00831d586c52)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/07b7fd12-53c1-4126-bd93-c9f6fa90ad92)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/c1dee3e4-7f49-4179-b2af-82bcc5de0c88)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/9ffb77e0-2a72-47df-ab87-c198bfecf82b)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/5c08c906-f798-40af-9748-2dcfd4f1fc24)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/75e7808d-64c3-4623-82ae-73dbf762f4dd)

3.Test results with complex correct Pyr code:
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/8e6aa38d-578f-42c1-bb2e-4168b9db85f9)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/2f39cfbb-6cfa-44ef-a2b6-84bbe8a789f9)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/be00d9a3-60e3-4945-a269-ac7edccece8b)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/70484437-0669-47b8-ae5e-4a134977d96b)
![image](https://github.com/0NPNM0/pyret_compiler_C/assets/98509588/3c295370-31f4-402d-b3ed-d57aeb3c62c6)

