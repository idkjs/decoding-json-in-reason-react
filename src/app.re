[%bs.raw {|require('./app.css')|}];

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self => {
    let dummyRepo: RepoData.repo = {
      stargazers_count: 27,
      full_name: "jsdf/reason-react-hacker-news",
      html_url: "https://github.com/jsdf/reason-react-hacker-news",
    };
    <div className="App">
      <h1> (ReasonReact.string("Decoding Graphql in Reason")) </h1>
      <RepoItem repo=dummyRepo />
    </div>;
  },
};