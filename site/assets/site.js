(function () {
  var navBtn = document.querySelector(".nav-toggle");
  var nav = document.getElementById("site-nav");
  if (navBtn && nav) {
    navBtn.addEventListener("click", function () {
      var open = nav.classList.toggle("is-open");
      navBtn.setAttribute("aria-expanded", open ? "true" : "false");
    });
  }

  var root = document.documentElement;
  var themeBtn = document.querySelector("[data-theme-toggle]");
  var themeMeta = document.querySelector('meta[name="theme-color"]');

  function syncThemeChrome() {
    var t = root.getAttribute("data-theme") === "light" ? "light" : "dark";
    if (themeBtn) {
      themeBtn.setAttribute(
        "aria-label",
        t === "light" ? "Switch to dark theme" : "Switch to light theme"
      );
      themeBtn.setAttribute("title", themeBtn.getAttribute("aria-label"));
    }
    if (themeMeta) {
      themeMeta.setAttribute("content", t === "light" ? "#f1f5f9" : "#2d1b69");
    }
  }

  function applyTheme(next) {
    root.setAttribute("data-theme", next);
    try {
      localStorage.setItem("pseudocode-theme", next);
    } catch (e) {}
    syncThemeChrome();
  }

  if (themeBtn) {
    syncThemeChrome();
    themeBtn.addEventListener("click", function () {
      var cur = root.getAttribute("data-theme") === "light" ? "light" : "dark";
      applyTheme(cur === "light" ? "dark" : "light");
    });
  }
})();
