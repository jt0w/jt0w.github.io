#let blog(body) = {
  html.html({
    html.head({
      html.link(rel: "stylesheet", href: "../style/reset.css")
      html.link(rel: "stylesheet", href: "../style/blog.css")
    })
    html.body({
      html.main(body)
    })
  })
}
