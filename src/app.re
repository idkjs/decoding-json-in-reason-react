/* defining our state type before defining our component */
/* this lets us get one repo */
/* type state = {repoData: option(RepoData.repo)}; */
/* this type returns an array */
type state = {repoData: option(array(RepoData.repo))};

/* define action type */
type action =
  | Loaded(RepoData.repo);

let component = ReasonReact.reducerComponent("App");

/* our dummy data */
let dummyRepos: array(RepoData.repo) = [|
  {
    stargazers_count: 27,
    full_name: "jsdf/reason-react-hacker-news",
    html_url: "https://github.com/jsdf/reason-react-hacker-news",
  },
  {
    stargazers_count: 93,
    full_name: "reasonml/reason-tools",
    html_url: "https://github.com/reasonml/reason-tools",
  },
|];

/*
 /* change this to whats below to get array instead of one item */
 let repoItem = (repoData: option(RepoData.repo)) =>
   switch (repoData) {
   | Some(repo) => <RepoItem repo />
   | None => ReasonReact.string("Loading")
   }; */
let repoItems = (repoData: option(array(RepoData.repo))) =>
  switch (repoData) {
  | Some(repos) =>
    ReasonReact.array(
      Array.map(
        (repo: RepoData.repo) => <RepoItem key=repo.full_name repo />,
        repos,
      ),
    )
  | None => ReasonReact.string("Loading")
  };

let make = _children => {
  ...component,
  initialState: () => {repoData: Some(dummyRepos)},
  reducer: ((), _) => ReasonReact.NoUpdate,
  render: ({state: {repoData}}) =>
    <div className="App">
      <h1> (ReasonReact.string("Decoding JSON in Reason")) </h1>
      (repoItems(repoData))
    </div>,
};