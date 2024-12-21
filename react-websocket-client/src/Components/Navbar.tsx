// Navbar.tsx

import { Link } from "react-router-dom";
// import "./Navbar.css"; // Import a CSS file for styling

import Container from 'react-bootstrap/Container';
import Nav from 'react-bootstrap/Nav';
import Navbar from 'react-bootstrap/Navbar';

const NavigationBar = () => {
  return (
    // <nav className="navbar">
    //   <div className="navbar-logo">Wordclock</div>
    //   <ul className="navbar-links">
    //     <li>
    //       <Link to="/wifi">Wifi</Link>
    //     </li>
    //     <li>
    //       <Link to="/settings">Settings</Link>
    //     </li>
    //     <li>
    //       <Link to="/colors">Colors</Link>
    //     </li>
    //   </ul>
    // </nav>

 <Navbar expand="lg" className="bg-body-tertiary">
<Container>
  <Navbar.Brand href="wifi">Wordclock by Uniquelymade</Navbar.Brand>
  <Navbar.Toggle aria-controls="basic-navbar-nav" />
  <Navbar.Collapse id="basic-navbar-nav">
    <Nav className="me-auto">
      <Nav.Link href="wifi">WiFi</Nav.Link>
      <Nav.Link href="settings">Settings</Nav.Link>
      <Nav.Link href="colors">Colors</Nav.Link>
      
    </Nav>
  </Navbar.Collapse>
</Container>
</Navbar>  


  );
};

export default NavigationBar;
