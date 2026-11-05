# O Labirinto em RayCaster

Jogo em estilo Wolfenstein 3D usando raycasting, em C++ com SFML 3.
Projeto de estudo em andamento: estou evoluindo a base do tutorial para um jogo próprio. O jogo próprio consistirá em um labirinto no qual o jogador deverá escapar de um minitauro

## Créditos
A base deste código vem do tutorial em vídeo (https://youtu.be/2JDzJoEscr0)
e do repositório https://github.com/terroo/raycaster, de Terminal Root,
licenciado sob BSL-1.0. As alterações realizadas até o momento consistem em:
- Adaptação para Windows + Visual Studio e SFML 3.1;
- Comentários em português explicando cada etapa (fórmulas, lógica dos raios, texturas).

## Status e planos
- [x] Renderização de paredes com textura e iluminação por distância
- [x] Minimapa e movimentação com colisão
- [ ] Criação da imagem do inimigo e paredes do mapa próprias
- [ ] Criação do inimigo e movimentação

## Controles
- W / S: andar para frente / trás
- A / D: girar a câmera

## Como rodar (Windows)
1. Baixe o SFML 3.1.0 (Visual C++ 17, 64-bit) em https://www.sfml-dev.org/download/
2. Crie a variável de ambiente `SFML_DIR` apontando para a pasta do SFML
3. Abra `Raycaster.slnx` no Visual Studio e selecione **Debug | x64**
4. Copie `sfml-graphics-d-3.dll`, `sfml-system-d-3.dll` e `sfml-window-d-3.dll`
   da pasta `bin` do SFML para a pasta onde o `.exe` é gerado
5. Execute (a pasta `resources/` precisa estar ao lado do projeto)
