# Termômetro Microcontrolado

Gabriel Chicchi Grunspan R.A.:23.01059-2

Luigi Pieroni da Silva R.A.:23.00761-3

O desafio proposto na disciplina de **Microcontroladores e Instrumentação** consistia no desenvolvimento de um sensor analógico capaz de acionar algum atuador (com exceção de um LED) utilizando o microcontrolador **RP2040**. Dentre as diversas possibilidades, optamos por desenvolver um **sensor de temperatura**. Entretanto, surgiu um desafio adicional: como construir um sensor de temperatura sem utilizar componentes especificamente projetados para essa finalidade, como termistores, sensores integrados ou até mesmo transistores?

A partir desse problema, iniciamos um estudo sobre alternativas para a medição de temperatura e constatamos que seria possível utilizar a **queda de tensão direta de um diodo semicondutor** como variável de medição. Conforme apresentado no **Gráfico 1**, extraído do datasheet do diodo **1N4007**, a tensão direta no diodo varia em função da temperatura de junção. Dessa forma, essa característica pode ser explorada para estimar a temperatura a partir da medição da tensão sobre o componente.

<p align="center">
<img width="608" height="367" alt="image" src="https://github.com/user-attachments/assets/204536ac-c5c9-44a5-887a-46ce562f7a1b" />
</p>

Embora essa característica possibilite a construção do sensor, a análise do gráfico também revela algumas limitações. Em determinadas regiões de operação, a curva apresenta comportamento exponencial, tornando pequenas variações de temperatura responsáveis por grandes alterações na tensão, o que comprometeria a precisão da medição. Por esse motivo, optamos por limitar a faixa de operação do sensor a uma região aproximadamente linear da curva, adequada para a aplicação proposta.

Outro aspecto importante observado foi que a variação da tensão direta do diodo é extremamente pequena, da ordem de alguns milivolts por grau Celsius. Para tornar essa variação compatível com a resolução do conversor A/D (ADC) do RP2040, foi necessário desenvolver um circuito amplificador. Para isso, utilizamos dois transistores em configuração **Darlington**, cujos cálculos de ganho são apresentados na figura a seguir.

<p align="center">
<img width="588" height="750" alt="image" src="https://github.com/user-attachments/assets/1c410d2f-f025-4833-8f1f-b271cf0a3d07" />
</p>

Os cálculos indicaram um ganho aproximado de **40.000**, permitindo amplificar suficientemente a pequena variação de tensão proveniente do diodo.

Após a definição do circuito amplificador, foi desenvolvido o circuito eletrônico completo. Para adequar o sinal de saída à faixa de operação do ADC do RP2040 (0 a 3,3 V), foi empregado um **divisor de tensão**. Os valores dos resistores desse divisor não foram obtidos por cálculo teórico, mas sim por meio de testes experimentais, buscando uma faixa de leitura adequada para as temperaturas desejadas.

Durante essa etapa experimental, verificou-se também que uma alimentação de **18 V** proporcionava a melhor resposta do circuito, tornando necessária a utilização de um regulador de tensão para alimentar o sistema de forma segura. O circuito eletrônico final é apresentado na figura abaixo.

<p align="center">
<img width="1091" height="607" alt="image" src="https://github.com/user-attachments/assets/42d7db12-4e00-4907-b818-cd08d6526061" />
</p>

Com o hardware concluído, iniciou-se a etapa de calibração do sensor. Para isso, foi utilizado um termômetro comercial como referência. Enquanto o sensor desenvolvido fornecia a tensão de saída, o termômetro de referência indicava a temperatura real do ambiente. Durante o experimento, foi realizada a aquisição automática desses dados em intervalos de um segundo, permitindo a construção da curva de calibração do sistema.

A partir dos dados obtidos, foram testados diferentes modelos de regressão, sendo escolhida uma **linha de tendência exponencial**, por apresentar o melhor coeficiente de determinação (**R²**) e, consequentemente, a melhor aproximação dos dados experimentais.

<p align="center">
<img width="565" height="306" alt="image" src="https://github.com/user-attachments/assets/849f6be7-5fbd-4551-aa61-f28be8511b63" />
</p>

A equação obtida durante a calibração foi então incorporada ao firmware do RP2040. Considerando que o conversor analógico-digital possui resolução de **12 bits** (4096 níveis) e tensão máxima de referência de **3,3 V**, foi desenvolvida uma equação que converte diretamente o valor lido pelo ADC na temperatura correspondente, permitindo sua exibição em tempo real no display.

<p align="center">
<img width="318" height="107" alt="image" src="https://github.com/user-attachments/assets/e0a55a9d-d543-4908-bd54-6c3d5f6ab038" />
</p>

Por fim, foi desenvolvida a estrutura mecânica do equipamento utilizando o **SolidWorks**, visando obter um invólucro compacto, ergonômico e adequado ao uso como termômetro portátil.

<p align="center">
<img width="360" height="493" alt="image" src="https://github.com/user-attachments/assets/4263699e-a196-4608-8e29-99b0e311af4b" />
</p>

O projeto resultou em um sensor de temperatura totalmente funcional, desenvolvido a partir de componentes discretos e utilizando princípios físicos dos dispositivos semicondutores para realizar a medição. Além de atender aos requisitos propostos pela disciplina, o trabalho proporcionou uma compreensão aprofundada sobre instrumentação eletrônica, condicionamento de sinais, calibração experimental e processamento de dados em microcontroladores.

**Documentação**

Link para a Apresentação: https://github.com/gchicchi/Termometro_Micro/blob/main/Apresenta%C3%A7%C3%A3o_Final.pptx

Link para o Relatório Técnico: https://github.com/gchicchi/Termometro_Micro/blob/main/Relat%C3%B3rio_Final.pdf

