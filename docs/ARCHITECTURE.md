## Security review (SonarQube 2026-11-13)

### Web:S5725 - Remote artifacts should not be used without integrity checks

- **Archivo:línea:** `flasher/index.html:957`
- **Severidad:** LOW
- **Por qué era un fallo:** el script `esp-web-tools` se cargaba desde un CDN remoto (unpkg) sin comprobación de integridad. HTTPS solo protege el tránsito; si el CDN o el paquete se comprometen, un atacante podría sustituir el código y ejecutarlo en el navegador de quien use el flasher (robo de datos, ejecución de código malicioso). Referencias: CWE-353, OWASP Top 10 2021 A8 (Software and Data Integrity Failures), OWASP 2017 A6.
- **Qué se cambió:** la URL flotante `esp-web-tools@10` se fijó a `esp-web-tools@10.5.2` (la misma versión que se servía hoy) y se añadieron `integrity="sha384-..."` y `crossorigin="anonymous"` (este último es necesario para que el navegador pueda verificar SRI).
- **Por qué no se pierde funcionalidad:** se carga la misma versión que ya resolvía `@10`, por lo que el comportamiento es idéntico. Comprobación manual: abrir `flasher/index.html` y verificar que el botón de instalación aparece y que la consola del navegador no muestra errores de integridad. Para actualizar la librería hay que cambiar a la vez la URL y el hash.

### Not changed - requires decision

Ninguno. No quedó ningún hallazgo sin cambiar.
