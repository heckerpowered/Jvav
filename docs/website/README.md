# Jvav website

The static website for https://jvav.heckerpowered.net, with English and
Simplified Chinese pages and the English working-draft PDF. No framework,
package installation, build step, or server runtime is required.

## Cloudflare Pages

Use these settings in the Pages project connected to this repository:

| Setting | Value |
| --- | --- |
| Framework preset | None |
| Root directory | `docs/website` |
| Build command | `exit 0` |
| Build output directory | `.` |

Replace the previous Astro build command and `dist` output setting with the
values above. Choose the production branch containing the website commit.
If the project root is left at the repository root instead, set the build
output directory to `docs/website`.

These settings follow the [Cloudflare Pages static HTML guide](https://developers.cloudflare.com/pages/framework-guides/deploy-anything/).
Keep the existing `jvav.heckerpowered.net` custom domain attached to the Pages
project. Repository files alone do not change the Cloudflare project settings.

The existing GitHub Pages workflow uploads this directory directly as a static
artifact. It no longer needs the former Astro starter or its npm dependencies.

## Local preview

Run from the repository root:

```sh
python3 -m http.server 4173 --bind 127.0.0.1 --directory docs/website
```

Open http://127.0.0.1:4173/. Serve the directory over HTTP rather than opening
HTML files directly, because the site uses JavaScript modules and root-relative
URLs.

## Content

- `/` and `/zh/`: English and Chinese homepages.
- `/tour/`, `/design/`, `/specification/`: introduction, language design, and
  specification pages, with corresponding routes under `/zh/`.
- `/visuals/` and `/visuals/forms/`: retained visual candidates in both languages.
- `/documents/J0001.pdf`: the English working draft.

Edit the HTML, CSS, and JavaScript files directly. The pages are already the
publishable output; there is no generated `dist` directory. Keep the two language
versions consistent. The PDF's pending design questions remain pending; the
member-function capability model and compilation targets are presented with
these current design limits in the site copy.
