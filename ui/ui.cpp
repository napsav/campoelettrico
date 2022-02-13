#include "ui.h"

bool initial_layout_done;
ImGuiID main_dock;
ImGuiID sidebar_dock;
ImGuiID sidebarBottom;

void renderUi(bool &pause, SDL_Window *gWindow, bool &darkMode, vector2 &intensitaMouse, std::vector<PuntoDelCampo> &punti, std::vector<Carica> &cariche, std::vector<Sorgente> &sorgenti, Graph &graficoCariche) {
  ImGui_ImplSDLRenderer_NewFrame();
  ImGui_ImplSDL2_NewFrame(gWindow);
  ImGui::NewFrame();

  if (darkMode) {
    ImGui::StyleColorsDark();
  } else {
    ImGui::StyleColorsLight();
  }

  ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(viewport, ImGuiDockNodeFlags_PassthruCentralNode);

  if (!initial_layout_done) {
    initial_layout_done = true;

    // Try to find the nodes if there is a loaded state
    ImGuiDockNode *root_node = ImGui::DockBuilderGetNode(dockspace_id);
    // Do we have a tree?
    if (root_node && root_node->ChildNodes[0] && root_node->ChildNodes[0]->ChildNodes[0] && root_node->ChildNodes[0]->ChildNodes[1]) {
      sidebarBottom = root_node->ChildNodes[0]->ChildNodes[1]->ID;
      sidebar_dock = root_node->ChildNodes[0]->ID;
    } else {
      ImGui::DockBuilderRemoveNode(dockspace_id);
      ImGuiID dock_id_main = ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
      ImGui::DockBuilderSetNodeSize(dock_id_main, viewport->GetWorkCenter());
      // ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Down, 0.20f, NULL, &dock_id_main);
      // ImGui::DockBuilderSetNodeSize(dock_id_bottom, ImVec2{viewport->GetWorkCenter().x, 100});
      ImGuiID dock_id_sidebar = ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Right, 0.70f, NULL, &dock_id_main);
      ImGuiID dock_id_sidebarBottom = ImGui::DockBuilderSplitNode(dock_id_sidebar, ImGuiDir_Down, 0.70f, NULL, &dock_id_sidebar);
      // bottom_dock = dock_id_bottom;
      sidebar_dock = dock_id_sidebar;
      main_dock = dock_id_main;
      sidebarBottom = dock_id_sidebarBottom;
      ImGui::DockBuilderFinish(dockspace_id);
    }
  }

  ImGui::SetNextWindowDockID(sidebar_dock, ImGuiCond_FirstUseEver);
  ImGui::Begin("Impostazioni");
  if (pause) {
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Stato della simulazione: IN PAUSA");
    if (ImGui::Button("Avvia"))
      pause = !pause;
  } else {
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Stato della simulazione: IN ESECUZIONE");
    if (ImGui::Button("Metti in pausa"))
      pause = !pause;
  }
  if (ImGui::Button("Rimuovi traiettorie delle cariche")) {
    graficoCariche.puntiDelGrafico.clear();
  }
  ImGui::Separator();
  ImGui::TextWrapped("Intensità campo elettrico nel cursore totale: %f N/C",
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
  ImGui::End();

  ImGui::SetNextWindowDockID(sidebarBottom, ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Cariche di prova")) {
    ImGui::Text("Aggiungi carica in una posizione precisa");
    ImGui::InputFloat("x", &caricaNuova.x, 0, SCREEN_WIDTH, "%f");
    ImGui::InputFloat("y", &caricaNuova.y, 0, SCREEN_HEIGHT, "%f");
    if (ImGui::Button("Aggiungi")) {
      addCaricaFunc(cariche, caricaNuova.x, caricaNuova.y);
    }
    ImGui::TextWrapped("Per creare una carica, puoi anche puntare con il mouse dove vuoi aggiungerla e premere il tasto 'spazio'.");
    ImGui::Separator();
    ImGui::BeginTable("cariche", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp);
    ImGui::TableSetupColumn("N");
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
    ImGui::Separator();
    if (ImGui::Button("Elimina cariche")) {
      clearCariche(cariche);
    }
  }
  ImGui::End();

  ImGui::SetNextWindowDockID(sidebarBottom, ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Sorgenti")) {
    ImGui::Text("Aggiungi sorgente in una posizione precisa");
    ImGui::InputFloat("x", &sorgenteNuova.x, 0, SCREEN_WIDTH, "%f");
    ImGui::InputFloat("y", &sorgenteNuova.y, 0, SCREEN_HEIGHT, "%f");
    ImGui::InputFloat("caricaSorgenteNuova", &caricaSorgenteNuova, -1e-20f, +1e-20f, "%e");
    if (ImGui::Button("Aggiungi")) {
      addSorgenteFunc(sorgenti, sorgenteNuova.x, sorgenteNuova.y);
    }
    ImGui::Separator();
    ImGui::BeginTable("sorgenti", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp);
    ImGui::TableSetupColumn("N");
    ImGui::TableSetupColumn("Intensità (C)");
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
    if (ImGui::Button("Elimina sorgenti")) {
      clearSorgenti(sorgenti);
    }
  }
  ImGui::End();

  // Rendering ImGUI
  //ImGui::ShowDemoWindow();

  ImGui::Render();

  ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}
