# 2025.1-Compiladores

Este repositório contém o código fonte de um compilador para uma linguagem de programação simples. O compilador foi desenvolvido como parte da disciplina de Compiladores na Universidade Estadual da Bahia em 2025.1.


# Especificação da Linguagem1 Cshort / Versão 1.0

Notação BNF Estendida
Nas regras léxicas e sintáticas descritas abaixo os caracteres.

<li>Alternativas são separadas por barras verticais, ou seja, 'a | b' significa "a ou b";</li>
<li>Colchetes indicam opcionalidade: '[ a ]' significa um a opcional, ou seja, "a | ε" (ε refere-se à cadeia vazia);</li>
<li>Chaves indicam repetição: '{ a }' significa " ε | a | aa | aaa | ...";</li>
<li>Parênteses indicam ocorrência de uma das alternativas: '(a | b | c)' significa obrigatoriedade de escolha de a ou b ou c.</li>


# AFD 
Afim de inicar o projeto foi criado o AFD abaixo:

![COMPILA](https://github.com/user-attachments/assets/3adc5945-364f-463a-9204-23b49a77823f)


# Como rodar o projeto?

<ul>
  <li>Faça o git clone do projeto;</li>
  <li>Na sua IDE de preferência, navegue ate a pasta;

```sh
cshort_lexer
```
  </li>
  <li>Rode o comando;
    
```sh
gcc main.c lexer.c -o cshort_lexer
```
</li>
  <li>Rode o comando ./cshort_lexer  </li>
      
```sh
./cshort_lexer
```
  <li> Após isso sera exibido no console da sua IDE a análise léxica. </li>
</ul>
