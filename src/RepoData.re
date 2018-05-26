type repo = {
  full_name: string,
  stargazers_count: int,
  html_url: string,
};
/* Works */
/* let parseRepoJson = (json: Js.Json.t) : repo => {
     full_name: Json.Decode.field("full_name", Json.Decode.string, json),
     stargazers_count:
       Json.Decode.field("stargazers_count", Json.Decode.int, json),
     html_url: Json.Decode.field("html_url", Json.Decode.string, json),
   }; */

/* better */
let parseRepoJson = (json: Js.Json.t) : repo =>
  Json.Decode.{
    full_name: field("full_name", string, json),
    stargazers_count: field("stargazers_count", int, json),
    html_url: field("html_url", string, json),
  };