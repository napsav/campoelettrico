#include "ui.h"

void renderUi(bool &pause, SDL_Window *gWindow, bool &darkMode, vector2 &intensitaMouse, std::vector<PuntoDelCampo> &punti, std::vector<Carica> &cariche, std::vector<Sorgente> &sorgenti) {
  ImGui_ImplSDLRenderer_NewFrame();
  ImGui_ImplSDL2_NewFrame(gWindow);
  ImGui::NewFrame();

  if (darkMode) {
    ImGui::StyleColorsDark();
  } else {
    ImGui::StyleColorsLight();
  }

  ImGui::Begin("Impostazioni");
  if (pause) {
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Stato della simulazione: IN PAUSA");
  } else {
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Stato della simulazione: IN ESECUZIONE");
  }
  ImGui::Separator();
  ImGui::Text("Intensità campo elettrico nel cursore totale: %f N/C",
              intensitaMouse.modulo);
  ImGui::Separator();

  ImGui::Checkbox("Log di debug", &abilitaLog);

  if (ImGui::CollapsingHeader("Opzioni principali",
                              ImGuiTreeNodeFlags_None)) {
    ImGui::Text("Massa cariche di prova");
    ImGui::InputFloat("massa", &massa, 1e-5f, 1000.0f, "%e");
    ImGui::Text("Coloumb carica di prova");
    ImGui::InputFloat("caricaDiProva", &caricaDiProva, 1e-20f, 1.0f, "%e");
    ImGui::Text("Linee di forza step max");
    ImGui::InputInt("maxStep", &maxStep, 1, 10000);

    ImGui::Text("Densità linee");
    ImGui::InputFloat("densitaLinee", &densitaLinee, 1e2, 1e24, "%e");

    ImGui::Text("Dettaglio linee di forza");
    ImGui::InputFloat("salto", &salto, 0.1, 100);

    ImGui::Text("Densità campo vettoriale");
    if (ImGui::SliderInt("densità", &densita, 4, 64))
      setDensity(punti, densita);
    ImGui::Text("Lunghezza vettori normalizzati");
    ImGui::SliderInt("lunghezza", &lunghezza, 1, 30);
    ImGui::Text("Raggio rappresentazione sorgenti");
    ImGui::SliderInt("raggio", &raggio, 1, 30);
  }
  if (ImGui::CollapsingHeader("Opzioni visuali", ImGuiTreeNodeFlags_None)) {
    ImGui::Text("Max Carica (Rendering)");
    ImGui::InputFloat("maxCarica", &maxCarica, 0.001, 0.1, "%e");
    ImGui::Text("Colore base (Rendering)");
    ImGui::SliderInt("coloreBase", &coloreBase, 0, 255);
    ImGui::Text("Scala pixel (Rendering)");
    ImGui::SliderInt("scala", &scala, 1, 1000);
    ImGui::Text("Raggio cariche di prova");
    ImGui::SliderInt("raggioCarica", &raggioCarica, 1, 100);
    ImGui::Checkbox("Griglia", &drawGrid);
    ImGui::Checkbox("Linee di campo", &drawLineeDiCampo);
    ImGui::Checkbox("Traiettoria cariche di prova", &drawGraficoCariche);
    ImGui::Checkbox("Campo vettoriale", &drawCampoVettoriale);
    ImGui::Checkbox("Sorgenti visibili", &drawSorgenti);
    ImGui::Checkbox("Sorgenti colorate", &sorgentiColoreSegno);
    ImGui::Checkbox("Modalità scura", &darkMode);
    ImGui::Separator();
    ImGui::ColorEdit4("Colore sfondo", &coloreSfondo[0]);
    ImGui::ColorEdit4("Colore cariche", &coloreCarica[0]);
    if (!sorgentiColoreSegno)
      ImGui::ColorEdit4("Colore sorgente", &coloreSorgente[0]);
    ImGui::ColorEdit4("Colore grafico cariche", &coloreGrCariche[0]);
    ImGui::ColorEdit4("Colore base vettori", &coloreBaseRGB[0]);
    ImGui::ColorEdit4("Colore griglia primario", &coloreGrigliaPrimario[0]);
    ImGui::ColorEdit4("Colore griglia secondario",
                      &coloreGrigliaSecondario[0]);
    ImGui::ColorEdit4("Colore linee di campo", &coloreLinee[0]);
  }

  ImGui::Separator();
  ImGui::Text("Aggiungi carica in una posizione precisa");
  ImGui::InputFloat("x", &caricaNuova.x, 0, SCREEN_WIDTH, "%f");
  ImGui::InputFloat("y", &caricaNuova.y, 0, SCREEN_HEIGHT, "%f");
  if (ImGui::Button("Aggiungi")) {
    addCaricaFunc(cariche, caricaNuova.x, caricaNuova.y);
  }
  ImGui::End();
  // TODO: resolve crash when collapsing source charges window
  // TODO: better handle placing of windows
  ImGui::Begin("Sorgenti");
  ImGui::BeginTable("sorgenti", 4, ImGuiTableFlags_Borders);
  ImGui::TableSetupColumn("N");
  ImGui::TableSetupColumn("Coloumb");
  ImGui::TableSetupColumn("X");
  ImGui::TableSetupColumn("Y");
  ImGui::TableHeadersRow();
  for (unsigned int i = 0; i < sorgenti.size(); i++) {

    std::ostringstream streamObj;
    streamObj << sorgenti[i].getCharge();
    // ImGui::Text("Sorgente %d: %s C", i, streamObj.str().c_str());

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("%d", i);
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%s", streamObj.str().c_str());
    ImGui::TableSetColumnIndex(2);
    ImGui::Text("%f", sorgenti[i].getPosition().x);
    ImGui::TableSetColumnIndex(3);
    ImGui::Text("%f", sorgenti[i].getPosition().y);
  }
  ImGui::EndTable();
  ImGui::Separator();
  ImGui::Text("Aggiungi sorgente in una posizione precisa");
  ImGui::InputFloat("x", &sorgenteNuova.x, 0, SCREEN_WIDTH, "%f");
  ImGui::InputFloat("y", &sorgenteNuova.y, 0, SCREEN_HEIGHT, "%f");
  ImGui::InputFloat("caricaSorgenteNuova", &caricaSorgenteNuova, -1e-20f, +1e-20f, "%e");
  if (ImGui::Button("Aggiungi")) {
    addSorgenteFunc(sorgenti, sorgenteNuova.x, sorgenteNuova.y);
  }
  ImGui::End();

  if (cariche.size() > 0) {
    ImGui::Begin("Cariche di prova");
    ImGui::BeginTable("cariche", 5, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn("Carica");
    ImGui::TableSetupColumn("Accelerazione x");
    ImGui::TableSetupColumn("Accelerazione y");
    ImGui::TableSetupColumn("Posizione x");
    ImGui::TableSetupColumn("Posizione y");
    ImGui::TableHeadersRow();
    for (unsigned int i = 0; i < cariche.size(); i++) {
      ImGui::TableNextRow();

      ImGui::TableSetColumnIndex(0);
      ImGui::Text("%d", i);
      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%f", cariche[i].getAcceleration().x);
      ImGui::TableSetColumnIndex(2);
      ImGui::Text("%f", cariche[i].getAcceleration().y);
      ImGui::TableSetColumnIndex(3);
      ImGui::Text("%f", cariche[i].getPosition().x);
      ImGui::TableSetColumnIndex(4);
      ImGui::Text("%f", cariche[i].getPosition().y);

      // ImGui::Text("Carica %d: ACC\tX=%f\tY=%f\tPOS\tX=%f\tY=%f", i,
      // cariche[i].getAcceleration().x, cariche[i].getAcceleration().y,
      // cariche[i].getPosition().x, cariche[i].getPosition().y);
    }
    ImGui::EndTable();
    ImGui::End();
  }

    // Rendering ImGUI
    ImGui::ShowDemoWindow();

    ImGui::Render();

    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}
