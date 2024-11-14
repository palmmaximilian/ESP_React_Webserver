import * as React from "react";
import ReactDOM from "react-dom/client";
import App from "./App.tsx";
import "./index.css";
import { BrowserRouter } from "react-router-dom";
import { WordclockDataProvider } from "./AppContext/AppContext.tsx";
import Navbar from "./Components/Navbar.tsx";

ReactDOM.createRoot(document.getElementById("🌐") as HTMLElement).render(
  <React.StrictMode>
    <WordclockDataProvider>
      <BrowserRouter>
        <Navbar /> 
        <App />
      </BrowserRouter>
    </WordclockDataProvider>
  </React.StrictMode>
);
