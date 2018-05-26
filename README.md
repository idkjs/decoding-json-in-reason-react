# Decoding API Response JSON in ReasonReact

Reason is a new statically-typed functional programming language from Facebook which can be compiled to Javascript. Reason React is a wrapper for React which makes it easy to use from Reason.

We're going to build a small single page web app to put Reason React through its paces. The app will display a list of top Reason-related Github repos. It's a small enough task that we can complete it in a few hours, but also has enough complexity that we can kick the tires of this new language. This tutorial expects no existing knowledge of Reason, though a basic familiarity with static types would be helpful.

# A New roject

We're going to use `create-react-app` using `reason-scripts` which will create a starting point for our app, which is going to be called `decoding-reason`:

```
yarn create react-app decoding-reason -- --scripts-version reason-scripts
```

Start the project by running `yarn start` which should open `https://localhost:3000` in your browser.

# A Record Type

We will be decoding a response from the public data from this Github API request: https://api.github.com/search/repositories?q=topic%3Areasonml&type=Repositories but first lets set it up using fake data.

Create a new file called RepoData.re and add this code into it.

```
type repo = {
  full_name: string,
  stargazers_count: int,
  html_url: string
};
```

# Files are modules

We've defined our type at the top level of the file.

> Top-level basically means visually, no indentation (very poor way of explaining it, but you get the idea). a let foo = 1 that’s not nested inside anything else is top-level. a let foo = 1 that’s inside a function body isn’t at the top-level. [@chenglou](https://reasonml.chat/t/bucklescript-top-level-declarations/399/3?u=idkjs)

In Reason, every file is a module, and all the things defined at the top level of the file using the keywords let, type, and module are exposed to be used from other files (that is, other modules). In this case, other modules can reference our repo type as RepoData.repo. Unlike in Javascript, no imports are required to reference things from other modules.

Let's use our type in app.re. The repos page is just a list of repos, with each item in the list consisting of the name of the repo (linking to the repo on Github), and the number of stars the repo has. We'll define some dummy data and sketch out a new component called `RepoItem` in `RepoItem.re` to represent an item in the list of repos:

```
//RepoItem.re
let component = ReasonReact.statelessComponent("RepoItem");

let make = (_children) => {
  ...component,
  render: (_self) => {
    /* our dummy data */
    let dummyRepo: RepoData.repo = {
      stargazers_count: 27,
      full_name: "jsdf/reason-react-hacker-news",
      html_url: "https://github.com/jsdf/reason-react-hacker-news"
    };

    <div className="App">
      <h1>{ReasonReact.stringToElement("Reason Projects")}</h1>
      <RepoItem repo=dummyRepo />
    </div>
  }
};
```

In the statement beginning `let dummyRepo: RepoData.repo = {...}`, `dummyRepo` is the name of the constant we're defining and `RepoData.repo` is the type we're annotating it with which is coming from where we defined it in `RepoData.re`. Remember that this module is available anywhere in the project. Reason can infer the types of most things we declare, but here it's useful to include the annotation so that the typechecker can let us know if we've made a mistake in our test data.

# Return values in Reason

Note that the body of the render function is now wrapped in {} braces, because it contains multiple statements. In Javascript, if we used braces around the body of an => arrow function we'd need to add a return statement to return a value. However in Reason, the value resulting from the last statement in the function, here:

```jsx
...
<div className="App">
      <h1>{ReasonReact.stringToElement("Reason Projects")}</h1>
      <RepoItem repo=dummyRepo />
</div>
```

automatically becomes the return value. If you don't want to return anything from a function, you can make the last statement `()` (which is called 'unit' in [Reason](https://reasonml.github.io/docs/en/function.html#optional-labeled-arguments)).

# Defining components in Reason React

You might now see an error saying The module or file `RepoItem`can't be found. That's because we added `<RepoItem repo=dummyRepo />` in the render function of the `App component`, but we haven't created that module yet. Add a new file called `RepoItem.re` containing:

```
let component = ReasonReact.statelessComponent("RepoItem");

let make = (~repo: RepoData.repo, _children) =>
  {
    ...component,
    render: (_self) =>
      <div>{ReasonReact.string(repo.full_name)}</div>
  };
```

What's going on here? Let's dissect what's happening in this file.

Each Reason React component is a Reason module which defines a function called make, which defines props and children arguments. The props are specified as [Labeled Arguments](https://reasonml.github.io/docs/en/function.html#labeled-arguments).

```
let component = ReasonReact.statelessComponent("SomeComponent");

let make = (~someProp, ~anotherProp, _children) => /* some stuff here */;
```

This `make` function returns a record. The first thing in this record is typically `...component`, where `component` is the return value of `ReasonReact.reducerComponent` or `ReasonReact.statelessComponent` (for components which do and don't use state, respectively). If this seems a bit weird, just think of it as inheriting from the React component class, like the equivalent of doing `class Foo extends React.Component {...` in ReactJS.

```
//RepoItem.re
let component = ReasonReact.statelessComponent("RepoItem");

let make = (~someProp, ~anotherProp, _children) =>
  {
    ...component,
    /* render and lifecycle methods go here */
  };
```

The rest of the record is where you can add the `render` function and the lifecycle methods you're used to from React.

So back to RepoItem.re:

```
let component = ReasonReact.statelessComponent("RepoItem");

let make = (~repo: RepoData.repo, _children) =>
  {
    ...component,
    render: (_self) =>
      <div>{ReasonReact.stringToElement(repo.full_name)}</div>
  };
```

What we have here is a `stateless component` which takes one prop called `repo`, (annotated with the `type repo` from the `RepoData` module), and renders a `div`.

In ReactJS, you can use `this.props` to access the component's props inside the `render` method. In ReasonReact we instead receive the props as labeled arguments to the `make` function, and we can use them inside our `render` function directly (as we are doing to access the `full_name` field of the `repo` prop above).

The `make` function also get passed a `children` argument, but we aren't making use of `children` in this component so we put an `_` underscore at the start of the `_children` argument name. That just lets Reason know we're not actually using that argument. Despite the fact we're not using this argument, it does still need to be included in the function arguments or you'll get an error.

Next we'll flesh out the `render` function to present the fields of the `repo` record:

```
let component = ReasonReact.statelessComponent("RepoItem");

let make = (~repo: RepoData.repo, _children) =>
  {
    ...component,
    render: (_self) =>
      <div className="RepoItem">
        <a href=repo.html_url>
          <h2>{ReasonReact.string(repo.full_name)}</h2>
        </a>
        {ReasonReact.string(string_of_int(repo.stargazers_count) ++ " stars")}
      </div>
  };
```
