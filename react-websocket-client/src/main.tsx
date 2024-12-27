import * as React from "react";
import ReactDOM from "react-dom/client";
import App from "./App.tsx";
// import "./index.css";
import { BrowserRouter } from "react-router-dom";
import { WordclockDataProvider } from "./AppContext/AppContext.tsx";
import NavigationBar from "./Components/Navbar.tsx";

ReactDOM.createRoot(document.getElementById("🌐") as HTMLElement).render(
  <React.StrictMode>
    <WordclockDataProvider>
      <BrowserRouter>
        <NavigationBar /> 
        <App />
      </BrowserRouter>
    </WordclockDataProvider>
  </React.StrictMode>
);
