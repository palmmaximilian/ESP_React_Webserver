// Navbar.tsx

import { Link } from "react-router-dom";
import "./Navbar.css"; // Import a CSS file for styling

const Navbar = () => {
  return (
    <nav className="navbar">
      <div className="navbar-logo">Wordclock</div>
      <ul className="navbar-links">
        <li>
          <Link to="/wifi">Wifi</Link>
        </li>
        <li>
          <Link to="/settings">Settings</Link>
        </li>
      </ul>
    </nav>
  );
};

export default Navbar;
