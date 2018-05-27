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

let parseReposResponseJson = json =>
  Json.Decode.field("items", Json.Decode.array(parseRepoJson), json);

let reposUrl = "https://api.github.com/search/repositories?q=topic%3Areasonml&type=Repositories";

let fetchRepos = () =>
  Js.Promise.(
    Fetch.fetch(reposUrl)
    |> then_(Fetch.Response.text)
    |> then_(jsonText =>
         resolve(parseReposResponseJson(Js.Json.parseExn(jsonText)))
       )
  );