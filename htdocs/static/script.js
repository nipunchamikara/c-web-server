document.addEventListener("DOMContentLoaded", () => {
  const appElement = document.getElementById("app");

  const heading = document.createElement("h1");
  heading.className = "display-5";
  heading.innerHTML = "This is a heading";

  const paragraph = document.createElement("p");
  paragraph.className = "lead";
  paragraph.innerHTML = "and this is a paragraph added with JavaScript";

  appElement.appendChild(heading);
  appElement.appendChild(paragraph);
});
